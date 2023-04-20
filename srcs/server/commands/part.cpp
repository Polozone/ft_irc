/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theodeville <theodeville@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 16:27:02 by theodeville       #+#    #+#             */
/*   Updated: 2023/03/27 08:25:59 by theodeville      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::getRealNameFromUserName(int client_fd, std::string &realname)
{
    const std::string str = getClientByFd(client_fd).getUsername().substr(getClientByFd(client_fd).getNickname().size() + 6, getClientByFd(client_fd).getUsername().size());
    
    realname = str;
    return (0);
}

int Server::partCommand(int client_fd, std::vector<std::string> clientInput)
{
    Channel *tmp = findChannelByName(clientInput[1]);
    Client client = getClientByFd(client_fd);
    if (!tmp)
    {
        std::cerr << "error: Channel not found\n";
        return (-1);
    }
    if (isValidFd(client_fd))
    {
        std::string realname;
        getRealNameFromUserName(client_fd, realname);
        tmp->removeClientByFd(client_fd);
        std::string message = ":" + getClientByFd(client_fd).getNickname() + "!" + realname + " PART " + tmp->getChannelName() + " :WeeChat 3.5\r\n";
        send(client_fd, message.data(), message.size(), 0);
        tmp->sendToAllClients(message, &client);
    }
    return (0);
}
