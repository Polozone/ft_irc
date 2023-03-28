/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theodeville <theodeville@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 10:59:09 by theodeville       #+#    #+#             */
/*   Updated: 2023/03/27 10:06:12 by theodeville      ###   ########.fr       */
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

int Server::checkNickUser(int client_fd, const std::string &nick)
{
    if (isValidFd(client_fd) == -1)
        return (-1);
    
    std::string user(getClientByFd(client_fd).getUsername());
    std::string newUser;
    int i = 0;
    int j = 0;
    
    while (user[i] != ' ')
        i++;
    
    const std::string userFirstParam(user.substr(j, i));
    if (userFirstParam != nick)
    {
        newUser = user.replace(j, i, nick);
        getClientByFd(client_fd).setUsername(newUser);
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
        checkNickUser(client_fd, nick);
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