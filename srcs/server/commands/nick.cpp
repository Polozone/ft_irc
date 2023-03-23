/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 10:59:09 by theodeville       #+#    #+#             */
/*   Updated: 2023/03/23 17:23:13 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::checkIfNickAvailable(const std::string &nick) const
{
    for (std::map<int, Client *>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
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
    Client  &target = getClientByFd(client_fd);

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
       target.sendMessage(ERR_NICKNAMEINUSE(getClientByFd(client_fd).getNickname()));
    }
    return (0);
}