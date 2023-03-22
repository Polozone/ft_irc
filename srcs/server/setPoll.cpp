/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setPoll.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theodeville <theodeville@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 13:10:58 by theodeville       #+#    #+#             */
/*   Updated: 2023/03/21 09:38:54 by theodeville      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

int Server::closeConnection(int i)
{
    std::cout << "connection closed - " << fds[i].fd << std::endl;
    close(fds[i].fd);
    fds.erase(fds.begin() + i);
    close_conn = 0;
    return (0);
}

int Server::handleCtrlD(const char *buffer)
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
        std::cout << concatenatedCmd << std::endl;
        concatenatedCmd.empty();
        return (1);
    }
    return (0);
}

void printStringInInt(const char *buffer)
{
    int i = 0;
    while (buffer[i])
    {
        printf("%d\n", buffer[i]);
        i++;
    }
}

int Server::readExistingConnection(int i)
{
    int status;
    char buffer[4056] = {0};

    status = recv(fds[i].fd, buffer, sizeof(buffer), 0);
    if (status < 0)
    {
        if (errno != EWOULDBLOCK)
        {
            perror("  recv() failed");
            close_conn = TRUE;
        }
        return (-1);
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
            setCommand(input, fds[i].fd);
            std::string tmp(buffer);
            std::cout << buffer << "\n";
            checkIfNewClient(buffer, fds[i].fd);
            // if (tmp.find("printpls") != std::string::npos)
            //     printClients();
            memset(buffer, 0, sizeof(buffer));
        }
    }

    return (0);
}

int Server::acceptIncomingConnection()
{
    int new_sd;
    do
    {
        new_sd = accept(listen_sd, NULL, NULL);
        if (new_sd < 0)
        {
            if (errno != EWOULDBLOCK)
            {
                perror("  accept() failed");
                end_server = TRUE;
            }
            return (-1);
        }

        fds.push_back(createPollFdNode(new_sd, POLLIN | POLLHUP));
    } while (new_sd != -1);
    return (0);
}

int Server::polling()
{
    int status;

    std::cout << "Waiting on poll()...\n";
    status = poll(fds.data(), fds.size(), 180000);
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

    //!----create poll instance assigning a fd to monitor\
    //!----and what tipe of event we want to monitor
    //!---- we add it to a list of fds, representing the users
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
                continue;
            }
            //! at this point if fd event different than POLLIN, we sent error 
            if (fds[i].revents != POLLIN)
            {
                printf("  Error! revents = %d\n", fds[i].revents);
                end_server = TRUE;
                break;
            }
            if (fds[i].fd == listen_sd)
            {
                // std::cout << fds[i].fd << " | listen_sd: " << listen_sd << "\n";
                if (acceptIncomingConnection() == -1)
                    break;
            }
            else
            {
                // std::cout << fds[i].fd << " | listen_sd: " << listen_sd << "\n";
                if (readExistingConnection(i) == -1)
                    break;
            }

            if (close_conn)
                closeConnection(i);
        }

    } while (end_server == FALSE);

    return (0);
}

void    Server::joinCommand(std::vector<std::string> command, int clientFd, Client *targetedClient)
{
    std::vector<std::string> channelList;
    std::vector<std::string> passwdList;


    if (command[1].find(',') != std::string::npos)
        channelList = split(command[1], ',');
    else
        channelList.push_back(command[1]);
    if (command.size() > 2)
    {
        if (command[2].find(',') != std::string::npos)
            passwdList = split(command[2], ',');
        else
            passwdList.push_back(command[2]);
    }
    std::vector<std::string>::iterator it;
    size_t i = 0;
    for (it = channelList.begin(); it != channelList.end(); ++it)
    {
        Channel *channel;
        if ((channel = findChannelByName(channelList[i], clientFd)) == NULL)
        {
            if (i < passwdList.size())
                channel = new Channel(channelList[i], passwdList[i], targetedClient);
            else
                channel = new Channel(channelList[i], "", targetedClient);
        }
        channel->addClientToChannel(clientFd, targetedClient);
        addToChannelList(channel);
        channel->printClientList();
        i++;
    }
}

void    Server::callCommand(std::vector<std::string> inputClient, int clientFd, Client *targetedClient)
{
    if (inputClient[0] == "JOIN")
        joinCommand(inputClient, clientFd, targetedClient);
    else if (inputClient[0] == "MODE")
        parseModeCommand(inputClient, clientFd);
    // else if (inputClient[0] == "INVITE")
    //     parseInviteCommand(inputClient, clientFd);
    else
        std::cout << "Command not found" << std::endl;
}

void    Server::setCommand(std::string &clientInput, int clientFd)
{
    std::vector<std::string> inputParsed;

    // for (_it = clients.begin(); _it != clients.end(); ++_it)
    // {
    //     std::cout << "fd == " << (*_it).second->getFd() << std::endl;
    // }
    dprintf(2, "test\n");
    Client *client = clients[findConnectedClientByFd(clientFd)];

    if (clientInput.empty())
        return ;

    std::string withoutExtraSpace = removeExtraSpaces(clientInput);
    inputParsed = split(withoutExtraSpace, ' ');

    if (inputParsed.size() < 2)
    {
        sendNumericReplies(clientFd, ERR_NEEDMOREPARAMS(client->getNickname()));
        return ;
    }
    callCommand(inputParsed, clientFd, client);
}
