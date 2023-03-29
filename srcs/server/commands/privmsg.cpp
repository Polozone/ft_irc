/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:47:54 by alexanderva       #+#    #+#             */
/*   Updated: 2023/03/27 15:29:21 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/*
 Function that extracts and concatenates strings from a vector starting at a 
 given index, with one space added between each concatenated string
 */
std::string extractAndConcatenateStrings(std::vector<std::string> strings, \
int index)
{
    std::string result = "";

    for (int i = index; i < strings.size(); i++)
    {
        result.append(strings[i]);
        result.append(" ");
    }
    // Remove the extra space at the end of the concatenated string
    if (!result.empty())
        result.erase(result.size() - 1);

    return result;
}

void Server::privmsgCommand(Client &client, std::vector<std::string> args)
{
    if (args.size() < 2)
    {
        // Send an error message to the client (e.g., ERR_NEEDMOREPARAMS)
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname()));
        return;
    }
    const std::string &target = args[1];
    const std::string &message = extractAndConcatenateStrings(args, 2);

    if (target.empty() || message.empty())
    {
        if (target.empty())
        {
            // Send ERR_NORECIPIENT error message (IRC numeric reply 411)
            client.sendMessage(ERR_NORECIPIENT(client.getNickname()));
        }
        if (message.empty())
        {
            // Send ERR_NOTEXTTOSEND error message (IRC numeric reply 412)
            client.sendMessage(ERR_NOTEXTTOSEND(client.getNickname()));
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
        std::string full_message = ":" + client.getNickname() + " PRIVMSG " \
        + target + " :" + message;

        // Send the message to all clients in the channel
        channel->sendToChannel(full_message, client);
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
        std::string full_message = ":" + client.getNickname() + " PRIVMSG " \
        + target_client->getNickname() + " :" + message;

        // Send the message to the target client
        target_client->sendMessage(full_message);
    }
}