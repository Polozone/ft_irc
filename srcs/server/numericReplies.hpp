
#pragma once

//------------------*ERROR-PRIVMSG-----------------------
# define ERR_NORECIPIENT(nickname) ":localhost 411 " + nickname + " :No recipient given\r\n"
# define ERR_NOTEXTTOSEND(nickname) ":localhost 473 " + nickname + " :No text to send\r\n"
# define ERR_NOSUCHNICK(nickname, nicktofind) ":localhost 401 " + nickname + " " + nicktofind + " :No such nick\r\n"
# define ERR_NOSUCHCHANNEL(nickname) ":localhost 403 "  + nickname + " :No such channel\r\n"
# define ERR_CANNOTSENDTOCHAN(nickname) ":localhost 404 " + nickname + " :Cannot send to channel\r\n"

//------------------*ERROR-OPER--------------------------
# define RPL_YOUREOPER(nickname) ":localhost 381 " + nickname + " :You are now an IRC operator\r\n"
# define ERR_PASSWDMISMATCH(nickname) ":localhost 464 " + nickname + " :Password incorrect\r\n"

//------------------*ERROR-MODE_USER--------------------------
# define ERR_USERSDONTMATCH(nickname) ":localhost 502 " + nickname + " :Cannot change mode for other users\r\n"
# define ERR_UMODEUNKNOWNFLAG(nickname) ":localhost 501 " + nickname + " :Unknown MODE flag\r\n"
# define RPL_UMODEIS(nickname, user_modes) ("221 " + nickname + " " + user_modes)

//------------------*ERROR-MODE_USER--------------------------
# define ERR_USERSDONTMATCH(nickname) ":localhost 502 " + nickname + " :Cannot change mode for other users\r\n"
# define ERR_UMODEUNKNOWNFLAG(nickname) ":localhost 501 " + nickname + " :Unknown MODE flag\r\n"
# define RPL_UMODEIS(nickname, user_modes) ("221 " + nickname + " " + user_modes)

# define ERR_NEEDMOREPARAMS(nickname) ":localhost 461 " + nickname + " number of params invalid\r\n"
# define ERR_INVITEONLYCHAN(nickname) ":localhost 473 " + nickname + ":Cannot join channel (+i)\r\n"
# define RPL_TOPIC(channel, topic) ":localhost 332  " + channel + " :" + topic + "\r\n"
# define ERR_WRONGPSSWD(channel) ":localhost 600 " + channel + ": password don't match\r\n"

# define RPL_NICK(oldnick, newnick) ":" + oldnick + " NICK " + newnick + "\r\n"
# define ERR_NICKNAMEINUSE(nickname) ":localhost 433 " + nickname + ":Nickname already in use\r\n"
# define ERR_ERRONEUSNICKNAME(port, nickname) ":localhost/" + port + " 432 " + nickname + " :Invalid Nickname\r\n"
# define RPL_NAMREPLY(username, channel, nickname) ":localhost 353 " + username + " " + channel + ":\r\n"
# define ERR_CHANOPRIVSNEEDED(nickname, channel) ":localhost 482 " + nickname + " " + channel + " :You're not channel operator\r\n"
# define RPL_WELCOME(port, nick) ":localhost/" + port + " 001 " + nick + " :Welcome to the server\r\n"
# define ERR_CHANNELISFULL(channel) ":localhost 471 " + channel + " :Cannot join channel (+l)"
# define ERR_NOPRIVILEGES ":localhost 481 :Permission Denied- You're not an IRC operator"
# define ERR_UNKNOWNMODE(mode) mode + " :is unknown mode char to me"

//------------------*ERROR-INVITE--------------------------
# define ERR_NOTONCHANNEL(channel, inviter) ": " + channel + " 442 "  +  inviter + " " + channel + " :You are not on that channel" 
# define ERR_CHANOPRIVSNEEDED(channel, inviter) ": " + channel + " 448 " + inviter + " " + channel + " :You are not on that channel"
# define ERR_USERONCHANNEL(channel, inviter) ": " + channel + " 443 " + inviter + " " + channel + " :is already on channel"
# define RPL_INVITING(channel, inviter, invitee) ": " + inviter + " 341 " + invitee + " " + channel + " :Inviting " + invitee + " to join " + channel
# define RPL_AWAY(channel, invitee) ":" + channel.name + " 301 " + " " + invitee.nickname + " :is away"