/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 12:17:29 by alexanderva       #+#    #+#             */
/*   Updated: 2023/04/05 12:53:19 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include    "../Server.hpp"

/**
    @brief Processes a NOTICE command sent by a client to the server.
    This function is called when a client sends a NOTICE command to the server. 
    The function extracts the command parameters from the inputClient vector 
    and sends the notice message to the target client.
    If the target client is not found or the target client is the same as the 
    sending client, no action is taken.
    @param client The client that sent the NOTICE command.
    @param inputClient The vector of input parameters sent with the NOTICE command.
    @return None.
    @note This function does not send any error replies in response to a NOTICE 
    command.
*/
void Server::noticeCommand(Client &client, const std::vector<std::string> &inputClient)
{
    // Do nothing since servers and clients must not send any error reply in response to a NOTICE command.
    if (inputClient.size() < 3)
        return;

    std::string msgTarget = inputClient[1];
    // Target is a user
    Client *target_client = findClientByNick(msgTarget);
    if ((!target_client) || !target_client->getNickname().compare(client.getNickname()))
        return;
    std::string text = inputClient[2];
    // Combine the remaining words if there are more than 3 words in the inputClient vector
    for (size_t i = 3; i < inputClient.size(); i++)
    {
        text += " " + inputClient[i];
    }
    std::string full_message = ":" + client.getNickname() + " NOTICE " + msgTarget + " :" + text;
    target_client->sendMessage(full_message);
}