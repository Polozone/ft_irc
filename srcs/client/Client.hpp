/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 07:46:30 by alexanderva       #+#    #+#             */
/*   Updated: 2023/03/16 08:50:45 by alexanderva      ###   ########.fr       */
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

// # include "../server/Server.hpp"

class Client 
{
private:
    // Data members
    const int               _fd;
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