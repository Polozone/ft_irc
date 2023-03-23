/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:47:54 by alexanderva       #+#    #+#             */
/*   Updated: 2023/03/23 15:32:30 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::msgCommand(Client &client, std::vector<std::string> args)
{
    if (args.size() < 2)
    {
        // Send an error message to the client (e.g., ERR_NEEDMOREPARAMS)
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname()));
        return;
    }
    const std::string &target = args[0];
    const std::string &message = args[1];

    if (target.empty() || message.empty())
    {
        if (target.empty())
        {
            // Send ERR_NORECIPIENT error message (IRC numeric reply 411)
            client.sendMessage(ERR_NORECIPIENT(client.getNickname()));
            // std::string error_message = ":" + server_.getServerName() +
            //                             " 411 " + client.getNickname() + " :No recipient given (PRIVMSG)";
            // client.sendMessage(error_message);
        }
        if (message.empty())
        {
            // Send ERR_NOTEXTTOSEND error message (IRC numeric reply 412)
            client.sendMessage(ERR_NOTEXTTOSEND(client.getNickname()));
            // std::string error_message = ":" + server_.getServerName() + " 412 " +
            //                             client.getNickname() + " :No text to send";
        }
        return;
    }

    if (target[0] == '#')
    { // Target is a channel
        Channel *channel = findChannelByName(target);
        if (!channel)
        {
            // Send an error message to the client (e.g., ERR_NOSUCHCHANNEL
            client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname()));
        }
        if (!channel->findClientByFd(client.getFd()))
        {
            // Send an error message to the client (e.g., ERR_CANNOTSENDTOCHAN)
            client.sendMessage(ERR_CANNOTSENDTOCHAN(client.getNickname()));
            return;
        }
        // Construct the message to be sent to the channel
        std::string full_message = ":" + client.getNickname() + "!~" + client.getUsername() \
        + "@" + client.getHostname() + " PRIVMSG " + target + " :" + message;

        // Send the message to all clients in the channel
        channel->sendToAllClients(full_message);
    }
    else
    {
        // Target is a user
        Client *target_client = findClientByNick(target);
        if (!target_client)
        {
            // Send an error message to the client (e.g., ERR_NOSUCHNICK)
            client.sendMessage(ERR_NOSUCHNICK(client.getNickname()));
            return;
        }
        // Construct the message to be sent to the target user
        std::string full_message = ":" + client.getNickname() + "!~" + \
        client.getUsername() + "@" + client.getHostname() + " PRIVMSG " + target_client->getNickname() + " :" + message;

        // Send the message to the target client
        target_client->sendMessage(full_message);
    }
}