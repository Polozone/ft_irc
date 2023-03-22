/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theodeville <theodeville@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 10:59:09 by theodeville       #+#    #+#             */
/*   Updated: 2023/03/22 08:34:55 by theodeville      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::checkIfNickAvailable(int client_fd, const std::string &nick)
{
    for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
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

// int Server::nickCommand(int client_fd, const std::string &nick, int newClient)
// {
//     if (newClient)
//     {
        
//     }
//     return (0);
// }