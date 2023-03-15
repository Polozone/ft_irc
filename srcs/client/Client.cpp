
#include "./Client.hpp"

Client::Client() {}

Client::Client(const std::string &user, const std::string &nick) :
    _user(user), _nick(nick) {}

Client::Client(const Client &rhs) {}

Client::~Client() {}

Client  &Client::operator=(const Client &rhs)
{
    _user = rhs._user;
    _user = rhs._nick;
    return (*this);
}
