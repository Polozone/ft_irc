/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 07:46:30 by alexanderva       #+#    #+#             */
/*   Updated: 2023/03/16 11:08:42 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <string>
# include <iostream>
# include <vector>
# include <deque>
# include <ctime>
# include <map>

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

// # include "../server/Server.hpp"

class Client 
{
private:
    // Data members
    int               _fd;
    std::string             _nickname; 
    std::string             _username;
    std::string             _fullname;
    std::string             _hostname;
    short                   _mode;
    bool                    _password;
    bool                    _authenticated;
    std::deque<std::string> _channelsJoined;

public:
    // Constructors and Destructor
    Client();
    Client(const int fd, std::string hostname);
    Client(const Client &src);
    ~Client();

    // Overloaded assignment operator
    Client &operator=(Client const &rhs);

    // Getters
    int getFd() const;
    std::string             getNickname() const;
    std::string             getUsername() const;
    std::string             getFullname() const;
    std::string             getHostname() const;
    short                   getMode() const;
    bool                    getPassword() const;
    bool                    getAuthenticated() const;
    std::deque<std::string> getChannelsJoined() const;

    // Setters
    void                    setFd(int fd);
    void                    setNickname(std::string nickname);
    void                    setUsername(std::string username);
    void                    setFullname(std::string fullname);
    void                    setHostname(std::string hostname);
    void                    setPassword(bool pass);
    void                    setAuthenticated(bool authenticated);

    // Mode methods
    void                    addMode(short mode);
    void                    removeMode(short mode);
    bool                    hasMode(short mode);

    // Channel methods
    bool                    addChannelJoined(std::string channelName);
    bool                    removeChannelJoined(std::string channelName);
};

// Overloaded output stream operator
std::ostream &operator<<(std::ostream &o, Client const &rhs);

#endif