/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 10:40:51 by alexanderva       #+#    #+#             */
/*   Updated: 2023/05/02 13:33:45 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

using namespace std;
#include    "../Server.hpp"

/* // Channel structure
struct Channel {
    string name;
    vector<string> members;
    bool inviteOnly;
    vector<string> operators;
};

// Client structure
struct Client {
    string nickname;
    vector<string> channels;
    bool away;
};
 */
// Invite function
void Server::inviteCommand(Client &inviter, std::vector<std::string> args)
{
    if (args.size() < 2)
    {
        // Send an error message to the client (e.g., ERR_NEEDMOREPARAMS)
        inviter.sendMessage(ERR_NEEDMOREPARAMS(inviter.getNickname()));
        return;
    }

    Client *invitee = findClientByNick(args[1]);
    Channel *channel = findChannelByName(args[2]);

    if (!invitee)
    {
        // Send an error message to the client (e.g., ERR_NOSUCHNICK)
        inviter.sendMessage(ERR_NOSUCHNICK(client.getNickname(), args[1]));
        return ;
    }
    if (!channel)
    {
        // Send an error message to the client (e.g., ERR_NOSUCHCHANNEL)
        inviter.sendMessage(ERR_NOSUCHCHANNEL(inviter.getNickname()));
        return ;
    }

    // Check if inviter is a member of the channel
    bool isMember = false;
    inviter.getChannelsJoined
    if ()
            
      isMember = true; 
    }
    if (!isMember)
    {
        // send numeric reply inviter in not in the channel
        inviter.sendMessage(ERR_NOTONCHANNEL(channel.getChannelName(), inviter.getNickname()));
        return;
    }

    // Check if channel is invite-only and if inviter is an operator
    if (channel.inviteOnly)
    {
        bool isOperator = false;
        for (unsigned int i = 0; i < channel.operators.size(); i++)
        {
            if (channel.operators[i] == inviter.nickname)
            {
                isOperator = true;
                break;
            }
        }
        if (!isOperator)
        {
            // send numirc reply inviter not an operator
            inviter.sendMessage(ERR_CHANOPRIVSNEEDED(channel.getChannelName(), inviter.getNickname()));
            return;
        }
    }

    // Check if invitee is already on the channel
    bool isOnChannel = false;
    for (unsigned int i = 0; i < invitee.channels.size(); i++)
    {
        if (invitee.channels[i] == channel.name)
        {
            isOnChannel = true;
            break;
        }
    }
    if (isOnChannel)
    {
        ERR_USERONCHANNEL(channel.getChannelName(), inviter.getNickname());
        return;
    }
    // Invite the user to the channel
    invitee.channels.push_back(channel.name);
    channel.members.push_back(invitee.nickname);
    // Notify the inviter and invitee of the invitation
    RPL_INVITING(channel.getChannelName(), inviter.getNickname(), invitee.getNickname());
    if (invitee.away)
}