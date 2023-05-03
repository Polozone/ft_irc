/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 10:40:51 by alexanderva       #+#    #+#             */
/*   Updated: 2023/05/03 10:20:12 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    string nickname; vector<string> channels;
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
    std::cout << "Passing by invite command\n";

    Client *invitee = findClientByNick(args[1]);
    Channel *channel = findChannelByName(args[2]);

    if (!invitee)
    {
        // Send an error message to the client (e.g., ERR_NOSUCHNICK)
        inviter.sendMessage(ERR_NOSUCHNICK(inviter.getNickname(), args[1]));
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
    std::vector<std::string> channelsJoint = inviter.getChannelsJoined(); 
    std::vector<std::string>::iterator it;
    for (it = channelsJoint.begin(); it != channelsJoint.end(); it++)
    {
        if (*it == channel->getChannelName())
            isMember = true;
    }
    if (!isMember)
    {
        // send numeric reply inviter in not in the channel
        inviter.sendMessage(ERR_NOTONCHANNEL(channel->getChannelName(), inviter.getNickname()));
        return;
    }

    // Check if channel is invite-only and if inviter is an operator
    if (channel->getInviteStatus())
    {
        if (!channel->isOperator(inviter.getNickname()))
        {
            // send numirc reply inviter not an operator
            inviter.sendMessage(ERR_CHANOPRIVSNEEDED(channel->getChannelName(), inviter.getNickname()));
            return;
        }
    }

    // Check if invitee is already on the channel
    channelsJoint = invitee->getChannelsJoined(); // we use the inviter vector used before 
    it = channelsJoint.begin(); // we use the same iterator used with inviter vector before 
    bool isOnChannel = false;
    for (; it != channelsJoint.end(); it++)
    {
        if (*it == channel->getChannelName())
        {
            isOnChannel = true;
            break;
        }
    }
    if (isOnChannel)
    {
        inviter.sendMessage(ERR_USERONCHANNEL(invitee->getNickname()));
        return;
    }
    // Invite the user to the channel
    invitee->addChannelJoined(channel->getChannelName());
    channel->addClientToChannelInvite(invitee->getFd(), invitee);
    // Notify the inviter and invitee of the invitation
    inviter.sendMessage(RPL_INVITING(channel->getChannelName(), inviter.getNickname(), invitee->getNickname()));
    // if (invitee.away)
}