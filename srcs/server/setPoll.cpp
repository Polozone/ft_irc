/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setPoll.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theodeville <theodeville@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 13:10:58 by theodeville       #+#    #+#             */
/*   Updated: 2023/03/20 10:02:19 by theodeville      ###   ########.fr       */
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
        std::cout << "EOF\n";
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

        std::cout << "Connection accepted - " << new_sd << std::endl;

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

    fds.push_back(createPollFdNode(listen_sd, POLLIN));

    do
    {
        if (polling() == -1)
            break;
        current_size = fds.size();
        for (int i = 0; i < current_size; i++)
        {
            if (fds[i].revents == 0)
                continue;
            if (fds[i].revents & POLLHUP)
            {
                closeConnection(i);
                continue;
            }
            if (fds[i].revents != POLLIN)
            {
                printf("  Error! revents = %d\n", fds[i].revents);
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
                closeConnection(i);
        }

    } while (end_server == FALSE);

    return (0);
}

void    Server::joinCommand(std::vector<std::string> command, int clientFd)
{
    std::vector<std::string> channelList;
    std::vector<std::string> passwdList;
    Client *client = clients[findConnectedClientByFd(clientFd)];

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
        if ((channel = findChannelByName(channelList[i])) == NULL)
        {
            if (i < passwdList.size())
                channel = new Channel(channelList[i], passwdList[i], client);
            else
                channel = new Channel(channelList[i], "", client);
            addToChannelList(channel);
        }
        channel->addClientToChannel(clientFd, client);
        channel->printClientList();
        i++;
    }
    // printChannelList();
}

void    Server::callCommand(std::vector<std::string> inputClient, int clientFd)
{
    if (inputClient[0] == "JOIN")
        joinCommand(inputClient, clientFd);
    else if (inputClient[0] == "MODE")
        parseModeCommand(inputClient, clientFd);
    else
        std::cout << "Command not found" << std::endl;
}

void    Server::setCommand(std::string &clientInput, int clientFd)
{
    std::vector<std::string> inputParsed;
    if (clientInput.empty())
        return ;
    std::string withoutExtraSpace = removeExtraSpaces(clientInput);
    inputParsed = split(withoutExtraSpace, ' ');
    if (inputParsed.size() < 2)
        return ;
    callCommand(inputParsed, clientFd);
}
