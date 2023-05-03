#include "./Server.hpp"

int Server::findFdsIndex(int fdToFind)
{
    std::vector<struct pollfd>::iterator it;
    std::vector<struct pollfd>::iterator ite = fds.end();
    int i = 0;

    for (it = fds.begin(); it != ite; ++it)
    {
        if (it->fd == fdToFind)
            return (i);
        i++;
    }
    return (-1);
}

int Server::closeConnection(int i)
{
    std::cout << "connection closed - " << fds[i].fd << std::endl;


    if (isValidFd(fds[i].fd) == -1)
        return (-1);

    Client client;

    client = getClientByFd(fds[i].fd);
    removeClientFromChannel(&client, fds[i].fd);

    removeClientFromMap(fds[i].fd);
    close(fds[i].fd);
    fds.erase(fds.begin() + i);
    close_conn = 0;
    return (0);
}

int Server::removeClientFromChannel(Client *toRemove, int fd)
{
    Channel *tmp;

    std::vector<std::string>::iterator it;
    std::vector<std::string>::iterator ite = toRemove->getChannelsJoined().end();

    for (it = toRemove->getChannelsJoined().begin(); it != ite; ++it)
    {
        tmp = findChannelByName(*it);
        if (tmp != NULL)
        {
            tmp->removeClientByFd(fd);
        }
    }
    return (0);
}

int Server::closeConnectionByFd(int fd)
{
    std::cout << "connection closed - " << fd << std::endl;

    Client client;
    if (isValidFd(fd) == -1)
        return (-1);

    client = getClientByFd(fd);
    removeClientFromChannel(&client, fd);

    removeClientFromMap(fd);
    close(fd);

    int index = findFdsIndex(fd);
    if (index == -1)
        return (-1);

    fds.erase(fds.begin() + index);
    close_conn = 0;
    return (0);
}

int Server::handleCtrlD(char *buffer)
{
    std::string tmp(buffer);
    if (detectEOF(buffer))
    {
        concatenate = 1;
        concatenatedCmd += tmp;
        return (1);
    }
    else if (!detectEOF(buffer) && concatenate)
    {
        concatenate = 0;
        concatenatedCmd += tmp;
    }
    return (0);
}

int Server::readExistingConnection(int i)
{
    int status;
    char buffer[4056] = {0};

    status = recv(fds[i].fd, buffer, sizeof(buffer), 0);
    if (status < 0)
    {
        int error_code;
        socklen_t error_code_size = sizeof(error_code);
        if (getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size) == -1)
        {
            perror("getsockopt() failed");
            close_conn = TRUE;
            return (-1);
        }
        if (error_code != EWOULDBLOCK)
        {
            std::cerr << "recv() failed with error code" << error_code << std::endl;
            close_conn = TRUE;
            return (-1);
        }
    }
    if (status == 0)
    {
        close_conn = TRUE;
    }
    if (status > 0)
    {
        if (!handleCtrlD(buffer))
        {
            std::string input(buffer);
            if (!concatenatedCmd.empty())
            {
                input = concatenatedCmd;
                concatenatedCmd = "";
            }
            if (checkIfNewClient(input.c_str(), fds[i].fd) > 0)
            {
                setCommand(input, fds[i].fd);
            }
            std::cout << buffer << std::endl;
            std::string tmp(buffer);
            memset(buffer, 0, sizeof(buffer));
        }
    }

    return (0);
}

// This function accepts incoming client connections on a listening socket
int Server::acceptIncomingConnection()
{
    socklen_t sin_size;             // Size of the client address structure
    struct sockaddr_in client_addr; // Client address structure
    int new_sd;

    // Clear the client address structure to all zeros
    memset(&client_addr, 0, sizeof(sockaddr_in));

    // Clear the size of the client address structure to all zeros
    memset(&sin_size, 0, sizeof(socklen_t));

    // Loop until a new client connection is accepted

    // Accept a new client connection on the listening socket
    // and get a new socket descriptor for the connection
    new_sd = accept(listen_sd, reinterpret_cast<struct sockaddr *>(&client_addr), &sin_size);
    // If the accept() call failed
    if (new_sd < 0)
    {
        perror("  accept() failed");
        // Set the end_server flag to TRUE
        end_server = TRUE;
        // Return -1 to indicate an error occurred
        return (-1);
    }
    // Get the local address and port of the new socket descriptor
    getsockname(new_sd, reinterpret_cast<struct sockaddr *>(&client_addr), &sin_size);

    // Create a new client object and add it to the _clientsTryingToConnect map
    try
    {
        this->_clientsTryingToConnect[new_sd] = new Client(new_sd, inet_ntoa(client_addr.sin_addr));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
    

    // Add a new pollfd structure to the fds vector for the new socket descriptor
    fds.push_back(createPollFdNode(new_sd, POLLIN | POLLHUP));

    std::cout << "Accepted connection - " << new_sd << std::endl;

    return (0);
}

int Server::polling()
{
    int status;
    nfds_t nfd;

    std::cout << "Waiting on poll()...\n";

    nfd = fds.size();
    status = poll(fds.data(), nfd, (3 * 60 * 1000));
    if (status < 0)
    {
        perror("poll()");
        return (-1);
    }

    if (status == 0)
    {
        std::cerr << "poll() timedout\n";
        return (-1);
    }
    return (0);
}

int Server::setPoll()
{
    int current_size;

    /*----create poll instance assigning a fd to monitor
      ----and what tipe of event we want to monitor
      ----we add it to a list of fds, representing the users*/
    fds.push_back(createPollFdNode(listen_sd, POLLIN));
    do
    {

        if (polling() == -1)
            break;
        current_size = fds.size();
        for (int i = 0; i < current_size; i++)
        {
            //! if no event
            if (fds[i].revents == 0)
                continue;
            //! if the file descriptor has hang up
            if (fds[i].revents & POLLHUP)
            {
                closeConnection(i);
                current_size--;
                continue;
            }
            //! at this point if fd event different than POLLIN, we sent error
            if (fds[i].revents != POLLIN)
            {
                end_server = TRUE;
                break;
            }
            if (fds[i].fd == listen_sd)
            {
                if (acceptIncomingConnection() == -1)
                    break;
            }
            else
            {
                if (readExistingConnection(i) == -1)
                    break;
            }

            if (close_conn)
            {
                closeConnection(i);
                break;
            }
        }
        std::cout << std::endl;

    } while (end_server == FALSE);

    return (0);
}