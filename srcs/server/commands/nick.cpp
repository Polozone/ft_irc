/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theodeville <theodeville@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 10:59:09 by theodeville       #+#    #+#             */
/*   Updated: 2023/03/22 09:26:02 by theodeville      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::checkIfNickAvailable(const std::string &nick) const
{
    for (std::map<int, Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (nick == it->second->getNickname())
        {
            std::cout << "Nick already in use\n";
            std::cout << "Have to send numeric reply\n";
            return (1);
        }
    }
    return (0);
}

int Server::nickCommand(int client_fd, const std::string &nick)
{
    if (!checkIfNickAvailable(nick))
    {
        getClientByFd(client_fd).setNickname(nick);
        std::cout << "Nick: " << getClientByFd(client_fd).getNickname() << std::endl;
    }
    return (0);
}