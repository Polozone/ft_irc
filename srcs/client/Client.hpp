#pragma once

#include <string>

class Client
{

public:
    Client();
    Client(const Client &rhs);
    ~Client();

    Client &operator=(const Client &rhs);

    // Setters
    void setFd(int fd) { _fd = fd; }
    void setNick(std::string nick) { _nick = nick; }
    void setUser(std::string user) { _user = user; }

    // Getters
    int getFd() const { return (_fd); }
    std::string getNick() const { return (_nick); }
    std::string getUser() const { return (_user); }

private:
    int _fd;
    std::string _nick;
    std::string _user;
};
