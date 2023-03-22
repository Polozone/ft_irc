/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mcdPrivMsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:51:11 by alexanderva       #+#    #+#             */
/*   Updated: 2023/03/22 16:01:58 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void PrivmsgCommand(Client &client, const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        // Send an error message to the client (e.g., ERR_NEEDMOREPARAMS)
            std::string error_message = ":" + server_.getServerName() +
                                        " 461 " + client.getNickname() + " :No recipient given (PRIVMSG)";
            client.sendMessage(error_message);
        return;
    }

    const std::string &target = args[0];
    const std::string &message = args[1];

    if (target.empty() || message.empty())
    {
        if (target.empty())
        {
            // Send ERR_NORECIPIENT error message (IRC numeric reply 411)
            std::string error_message = ":" + server_.getServerName() +
                                        " 411 " + client.getNickname() + " :No recipient given (PRIVMSG)";
            client.sendMessage(error_message);
        }

        if (message.empty())
        {
            // Send ERR_NOTEXTTOSEND error message (IRC numeric reply 412)
            std::string error_message = ":" + server_.getServerName() + " 412 " +
                                        client.getNickname() + " :No text to send";
            client.sendMessage(error_message);
        }

        return;
    }

    if (target[0] == '#')
    { // Target is a channel
        Channel *channel = server_.findChannel(target);
        if (!channel)
        {
            // Send an error message to the client (e.g., ERR_NOSUCHCHANNEL)
            return;
        }

        if (!channel->hasClient(client))
        {
            // Send an error message to the client (e.g., ERR_CANNOTSENDTOCHAN)
            return;
        }

        // Construct the message to be sent to the channel
        std::string full_message = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + client.getHostname() + " PRIVMSG " + target + " :" + message;

        // Send the message to all clients in the channel
        channel->sendMessage(full_message, &client);
    }
    else
    { // Target is a user
        Client &target_client = server_.findClient(target);
        if (!target_client)
        {
            // Send an error message to the client (e.g., ERR_NOSUCHNICK)
            return;
        }

        // Construct the message to be sent to the target user
        std::string full_message = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + client.getHostname() + " PRIVMSG " + target_client->getNickname() + " :" + message;

        // Send the message to the target client
        target_client->sendMessage(full_message);
    }
}