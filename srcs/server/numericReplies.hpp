//  ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
//            ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
//            ERR_CHANNELISFULL               ERR_BADCHANMASK
//            ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
//            RPL_TOPIC
#pragma once

//------------------*ERROR-JOIN-----------------------

# define ERR_NEEDMOREPARAMS(nickname) ":localhost 461 " + nickname + "number of params invalid"
# define ERR_INVITEONLYCHAN(nickname) ":localhost 473 " + nickname + ":Cannot join channel (+i)"


//------------------*ERROR-PRIVMSG-----------------------
# define ERR_NORECIPIENT(nickname) ":localhost 411 " + nickname + ":No recipient given"
# define ERR_NOTEXTTOSEND(nickname) ":localhost 473 " + nickname + ":No text to send"
# define ERR_NOSUCHCHANNEL(nickname) ":localhost 403" +   + nickname + "No such channel"
# define ERR_CANNOTSENDTOCHAN(nickname) ":localhost 404" +   + nickname + "Cannot send to channel"
# define ERR_ERR_NOSUCHNICK(nickname) ":localhost 406" +   + nickname + "There was not such nickname"

// # define ERR_BANNEDFROMCHAN(nickname) ":localhost/" +   + nickname + "user ban from channel"
// # define ERR_INVITEONLYCHAN(nickname) ":localhost/" +   + nickname + "number of params invalid"              
// # define ERR_BADCHANNELKEY(nickname) ":localhost/" +   + nickname + "number of params invalid"
// # define ERR_CHANNELISFULL(nickname) ":localhost/" +   + nickname + "number of params invalid"               
// # define ERR_BADCHANMASK(nickname) ":localhost/" +   + nickname + "number of params invalid"
// # define ERR_NOSUCHCHANNEL(nickname) ":localhost/" +   + nickname + "number of params invalid"               
// # define ERR_TOOMANYCHANNELS(nickname) ":localhost/" +   + nickname + "number of params invalid"
// # define RPL_TOPIC(nickname) ":localhost/" +   + nickname + "number of params invalid"