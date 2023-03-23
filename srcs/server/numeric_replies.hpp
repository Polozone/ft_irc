
#pragma once

#include "Server.hpp"

//------------------*ERROR-JOIN-----------------------

# define ERR_NEEDMOREPARAMS(command) ":localhost 461 " + command + " :not enough parameters\r\n"
# define ERR_INVITEONLYCHAN(nickname) ":localhost 473 " + nickname + ":Cannot join channel (+i)\r\n"
# define ERR_NICKNAMEINUSE(nickname) ":localhost 433 " + nickname + ":Nickname already in use\r\n"
# define ERR_NOSUCHCHANNEL(nickname) ":localhost " + nickname + "number of params invalid\r\n"
# define ERR_CHANNELISFULL(channel) ":localhost " + channel + " :Cannot join channel (+l)\r\n"
# define RPL_TOPIC(channel, topic) ":localhost 332  " + channel + " :" + topic + "\r\n"
# define RPL_NAMREPLY(username, channel, nickname) ":localhost 353 " + username + " " + channel + ":" + "awd awdd ddwa cawca" + "\r\n"
# define ERR_CHANOPRIVSNEEDED(channel) ":localhost/" + channel + " :You're not channel operator\r\n"


        // 482     ERR_CHANOPRIVSNEEDED "<channel> :You're not channel operator"
//   "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"


// ":localhost/" + sPort + " 001 " +
//                                       clientsTryingToConnect[client_fd]->getNickname() + " :Welcome to the server\r\n";