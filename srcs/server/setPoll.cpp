/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setPoll.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theodeville <theodeville@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 13:10:58 by theodeville       #+#    #+#             */
/*   Updated: 2023/03/12 14:47:06 by theodeville      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"
#include "../utils/string_utils.cpp"

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

int Server::readExistingConnection(int i)
{
    int status;
    char buffer[1026] = {0};

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
        return (0);
    }
    if (status > 0)
    {
        if (!handleCtrlD(buffer))
        {
            std::string input(buffer);
            setCommand(input);
            memset(buffer, 0, sizeof(buffer));
        }
        return (0);
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

void    Server::joinCommand()
{
    std::vector<std::string> channelList;
    std::vector<std::string> passwdList;
    if (_command[1].find(',') != std::string::npos)
        channelList = split(_command[1], ',');
    else
        channelList[0] = _command[1];
    if (!_command[2].empty())
    {
        if (_command[2].find(',') != std::string::npos)
            passwdList = split(_command[2], ',');
        else
            passwdList[0] = _command[2];
    }
    size_t index = 0;
    for (std::vector<std::string>::iterator it = channelList.begin(); it != channelList.end(); ++it)
    {
        if (passwdList[index].empty())
        {
            Channel *newChannel = new Channel(channelList[index], "");
            std::cout << newChannel->getPasswd() << " " << newChannel->getChannelName() << std::endl;
        }
        else
        {
            Channel *newChannel = new Channel(channelList[index], passwdList[index]);
            std::cout << newChannel->getPasswd() << " " << newChannel->getChannelName() << std::endl;
        }
        // std::cout << newChannel->getPasswd() << " " << newChannel->getChannelName() << std::endl;
        index++;
        // _ptrToChannel.push_back()
    }
    // for (std::vector<std::string>::iterator it = passwdList.begin(); it != passwdList.end(); ++it)
    //     std::cout << *it << std::endl;
    //Channel *channel = new Channel();
}

void    Server::modeCommand()
{
    std::cout << "mode" << std::endl;
}

void    Server::callCommand()
{
    if (_command[0] == "JOIN")
        joinCommand();
    else if (_command[0] == "MODE")
        modeCommand();
    else
        std::cout << "Command not found" << std::endl;
}

void    Server::setCommand(std::string &userInput)
{
    std::string withoutExtraSpace = removeExtraSpaces(userInput);
    _command = split(withoutExtraSpace, ' ');

    std::vector<std::string>::iterator it;
    callCommand();
    // std::cout << _command[0] << std::endl;
    // for (it = _command.begin(); it != _command.end(); ++it)
    // {
    //     std::cout << *it << std::endl;
    // }
}