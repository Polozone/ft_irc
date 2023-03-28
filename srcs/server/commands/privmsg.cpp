/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:47:54 by alexanderva       #+#    #+#             */
/*   Updated: 2023/03/28 09:08:08 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/**
    @brief This function extracts and concatenates all the strings in the \ 
    input vector starting from a given index.
    @param strings A vector of strings representing the strings to be \ 
    concatenated.
    @param index An integer representing the starting index of the vector from\ 
     where the strings will be concatenated.
    @return A string representing the concatenated result of all the strings \ 
    in the input vector starting from the given index.
    @note This function removes any extra space at the end of the concatenated\ 
     string.
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

/**
    @brief This function implements the IRC PRIVMSG command, which sends a\ 
    message to either a user or a channel.
    @param client A reference to a Client object representing the client who\ 
     issued the PRIVMSG command.
    @param args A vector of strings representing the arguments passed to the\ 
    PRIVMSG command.
    @note This function sends error messages to the client if the PRIVMSG\ 
    command is missing required parameters or if the target channel or user\ 
    does not exist.
*/
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