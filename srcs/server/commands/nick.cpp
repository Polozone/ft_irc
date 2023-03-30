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
    
    while (user[i] && user[i] != ' ')
        i++;
    
    const std::string userFirstParam(user.substr(j, i));
    std::cout << userFirstParam << std::endl;
    std::cout << nick << std::endl;
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
        const std::string oldnick(getClientByFd(client_fd).getNickname());
        getClientByFd(client_fd).setNickname(nick);
        const std::string newnick(getClientByFd(client_fd).getNickname());
        checkNickUser(client_fd, nick);
        sendNumericReplies(client_fd, RPL_NICK(oldnick, newnick));
    }
    else
    {
       target.sendMessage(ERR_NICKNAMEINUSE(getClientByFd(client_fd).getNickname()));
    }
    return (0);
}
