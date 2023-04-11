/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 08:11:26 by alexanderva       #+#    #+#             */
/*   Updated: 2023/04/04 08:52:53 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Client.hpp"

//! CONSTRUCTORS
Client::Client(void) : _nickname("*"), 
			_username(""), _fullname(""), _hostname(""), 
			_mode(none),  _password(""), _authenticated(false), _channelsJoined() { }

Client::Client(const int fd, std::string hostname) : _fd(fd), _nickname("*"), 
			_username(""), _fullname(""), _hostname(hostname), 
			_mode(none),  _password(""), _authenticated(false), _channelsJoined() { }

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
std::string Client::getPassword(void)                   const { return this->_password; }
bool Client::getAuthenticated(void)                     const { return this->_authenticated;}
std::deque<std::string> Client::getChannelsJoined(void) const { return this->_channelsJoined;}
bool Client::hasMode(short mode)                              { return ((this->_mode & mode) > 0); }
bool Client::getOperatorStatus() 						const { return (this->_operatorStatus); }

//! SETTERS
void Client::setFd(int fd) { this->_fd = fd; }
void Client::setNickname(const std::string &nickname) { this->_nickname = nickname; }
void Client::setUsername(const std::string &username) { this->_username = username; }
void Client::setFullname(const std::string &fullname) { this->_fullname = fullname; }
void Client::setHostname(const std::string &hostname) { this->_hostname = hostname; }
void Client::setPassword(const std::string &password) { this->_password = password; }
void Client::setAuthenticated(bool authenticated){this->_authenticated = authenticated;}
void Client::addMode(short mode) { this->_mode |= mode; }
void Client::removeMode(short mode) { this->_mode &= ~mode; }
void Client::setOperatorStatus(bool status) 		  {this->_operatorStatus = status; }

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

void Client::sendMessage(const std::string &message)
{
	std::string full_message = message + "\r\n"; // Add CRLF to the message as per the IRC protocol
	ssize_t bytes_sent = 0;
	ssize_t total_bytes_sent = 0;
	const char *buffer = full_message.c_str();
	size_t buffer_length = full_message.length();

	// Send the message to the client through the client's socket (client_fd_)
	while (total_bytes_sent < buffer_length)
	{
		bytes_sent = send(_fd, buffer + total_bytes_sent, buffer_length - total_bytes_sent, 0);
		if (bytes_sent == -1)
		{
			std::cerr << "Error sending information to : " << _nickname << "\n";
			break;
		}
		total_bytes_sent += bytes_sent;
	}
	// std::cout << buffer << "\n";
}

//! OSTREAM 
std::ostream & operator<<(std::ostream &o, Client const &rhs) {
	if (rhs.getAuthenticated() == true)
		o << rhs.getNickname() << "!" << rhs.getUsername();
	return o;
}
