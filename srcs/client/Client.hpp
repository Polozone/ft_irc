#pragma once

#include <string>

class Client
{

public:
    Client();
    Client(const std::string &user, const std::string &nick);
    Client(const Client &rhs);
    ~Client();

    Client  &operator=(const Client &rhs);

private:

    std::string _user;
    std::string _nick;
    int         _fd;

};
