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
    void setNick(const std::string &nick) { _nick = nick; }
    void setUser(const std::string &user) { _user = user; }
    void setPassword(const std::string &password) { _password = password; }

    // Getters
    int getFd() const { return (_fd); }
    std::string getNick() const { return (_nick); }
    std::string getUser() const { return (_user); }
    std::string getPassword() const { return (_password); }

private:
    int _fd;
    std::string _nick;
    std::string _user;
    std::string _password;
};
