//  ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
//            ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
//            ERR_CHANNELISFULL               ERR_BADCHANMASK
//            ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
//            RPL_TOPIC
#pragma once

#include "Server.hpp"

//------------------*ERROR-JOIN-----------------------

# define ERR_NEEDMOREPARAMS(nickname) ":localhost 461 " + nickname + "number of params invalid"
# define ERR_INVITEONLYCHAN(nickname) ":localhost 473 " + nickname + ":Cannot join channel (+i)"

// # define ERR_BANNEDFROMCHAN(nickname) ":localhost/" +   + nickname + "user ban from channel"
// # define ERR_INVITEONLYCHAN(nickname) ":localhost/" +   + nickname + "number of params invalid"              
// # define ERR_BADCHANNELKEY(nickname) ":localhost/" +   + nickname + "number of params invalid"
// # define ERR_CHANNELISFULL(nickname) ":localhost/" +   + nickname + "number of params invalid"               
// # define ERR_BADCHANMASK(nickname) ":localhost/" +   + nickname + "number of params invalid"
// # define ERR_NOSUCHCHANNEL(nickname) ":localhost/" +   + nickname + "number of params invalid"               
// # define ERR_TOOMANYCHANNELS(nickname) ":localhost/" +   + nickname + "number of params invalid"
// # define RPL_TOPIC(nickname) ":localhost/" +   + nickname + "number of params invalid"