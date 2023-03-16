/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 08:11:26 by alexanderva       #+#    #+#             */
/*   Updated: 2023/03/16 08:13:32 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Client.hpp"

//!   The available modes are:
//		none												=> 0	0000 0000
//		a - user is flagged as away;						=> 1	0000 0001
//		w - user receives wallops;							=> 2	0000 0010			
//     	i - marks a users as invisible; 					=> 3	0000 0100
//      o - operator flag;  								=> 4	0000 1000
//      B - marks a bot;									=> 5	0001 0000
//      s - marks a user for receipt of server notices.		=> 6	0010 0000
//      r - restricted user connection;	 					=> 7	0100 0000

#define MOD_NONE        (0 << 0)
#define MOD_AWAY        (1 << 0)
#define MOD_WALLOPS     (1 << 2)
#define MOD_INVISIBLE   (1 << 3)
#define MOD_OPER        (1 << 4)
#define MOD_BOT    		(1 << 5)
#define MOD_SRVNOTICES  (1 << 6)
#define MOD_RESTRICTED  (1 << 7)

//! CONSTRUCTORS
Client::Client(const int fd, std::string hostname) : _fd(fd), _nickname("*"), 
			_username(""), _fullname(""), _hostname(hostname), 
			_mode(MOD_NONE),  _password(false), _authenticated(false), _channelsJoined() { }

Client::Client(const Client &src) : _fd(src._fd)
{
    *this = src;
}

// DESTRUCTORS
Client::~Client() { }

// ASSIGN OVERLOAD
Client& Client::operator=(Client const &rhs)
{
    if (this != &rhs)
    {
        this->_nickname = rhs._nickname;
        this->_username = rhs._username;
        this->_fullname = rhs._fullname;
        this->_hostname = rhs._hostname;
        this->_mode = rhs._mode;
        this->_password = rhs._password;
        this->_authenticated = rhs._authenticated;
        this->_channelsJoined = rhs._channelsJoined;
    }
    return *this;
}

//! GETTERS
int Client::getFd(void) const { return this->_fd; }
std::string Client::getNickname(void)                   const { return this->_nickname; }
std::string Client::getUsername(void)                   const { return this->_username; }
std::string Client::getFullname(void)                   const { return this->_fullname; }
std::string Client::getHostname(void)                   const { return this->_hostname; }
short Client::getMode(void)                             const { return this->_mode; }
bool Client::getPassword(void)                          const { return this->_password; }
bool Client::getAuthenticated(void)                     const { return this->_authenticated;}
std::deque<std::string> Client::getChannelsJoined(void) const { return this->_channelsJoined;}
bool Client::hasMode(short mode)                              { return ((this->_mode & mode) > 0); }

//! SETTERS
void Client::setNickname(std::string nickname) { this->_nickname = nickname; }
void Client::setUsername(std::string username) { this->_username = username; }
void Client::setFullname(std::string fullname) { this->_fullname = fullname; }
void Client::setHostname(std::string hostname) { this->_hostname = hostname; }
void Client::setPassword(bool password) { this->_password = password; }
void Client::setAuthenticated(bool authenticated) { 
	this->_authenticated = authenticated; 
}
void Client::addMode(short mode) { this->_mode |= mode; }
void Client::removeMode(short mode) { this->_mode &= ~mode; }

//! CHANNEL JOINED MANAGEMENT
bool Client::addChannelJoined(std::string channelName) {
	std::deque<std::string>::iterator it;
	std::deque<std::string>::iterator ite = this->_channelsJoined.end();

	for (it = this->_channelsJoined.begin(); it < ite; ++it) {
		if (*it == channelName)
			return false;
	}
	this->_channelsJoined.push_back(channelName);
	return true;
}

bool Client::removeChannelJoined(std::string channelName) {
	std::deque<std::string>::iterator it;
	std::deque<std::string>::iterator ite = this->_channelsJoined.end();

	for (it = this->_channelsJoined.begin(); it < ite; ++it) {
		if (*it == channelName) {
			this->_channelsJoined.erase(it);
			return true;
		}
	}
	return false;
}

//! OSTREAM 
std::ostream & operator<<(std::ostream &o, Client const &rhs) {
	if (rhs.getAuthenticated() == true)
		o << rhs.getNickname() << "!" << rhs.getUsername();
	return o;
}
