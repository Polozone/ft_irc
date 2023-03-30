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

int Server::partCommand(int client_fd, std::vector<std::string> clientInput)
{
    Channel *tmp = findChannelByName(clientInput[1]);
    if (!tmp)
    {
        std::cerr << "error: Channel not found\n";
        return (-1);
    }
    if (isValidFd(client_fd))
    {
        tmp->removeClientByFd(client_fd);
        std::string message = ":" + getClientByFd(client_fd).getNickname() + "!tdeville PART " + tmp->getChannelName() + " :WeeChat 3.5\r\n";
        std::cout << "Message: " << message << std::endl;
        send(client_fd, message.data(), message.size(), 0);
        tmp->sendToAllClients(message);
    }
    return (0);
}