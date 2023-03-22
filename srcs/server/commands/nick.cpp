/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theodeville <theodeville@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 10:59:09 by theodeville       #+#    #+#             */
/*   Updated: 2023/03/22 14:26:20 by theodeville      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::checkIfNickAvailable(const std::string &nick) const
{
    for (std::map<int, Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (nick == it->second->getNickname())
        {
            return (1);
        }
    }
    return (0);
}

int Server::nickCommand(int client_fd, const std::string &nick)
{
    if (isValidFd(client_fd) == -1)
        return (-1);
    if (!checkIfNickAvailable(nick))
    {
        getClientByFd(client_fd).setNickname(nick);
        const std::string sPort(port);
        const std::string welcomeClient = ":localhost/" + sPort + " 001 " +
                                          getClientByFd(client_fd).getNickname() + " :Nick setup\r\n";
        if (send(client_fd, welcomeClient.data(), welcomeClient.size(), 0) < 0)
        {
            std::cerr << "Send error\n";
            return (-1);
        }
    }
    else
    {
        sendNumericReplies(client_fd, ERR_NICKNAMEINUSE(getClientByFd(client_fd).getNickname()));
    }
    return (0);
}