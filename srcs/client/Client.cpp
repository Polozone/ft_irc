#include "./Client.hpp"

Client::Client() : _fd(0), _nick(""), _user("") {}
Client::Client(const Client &rhs) { *this = rhs; }
Client::~Client() {}

Client &Client::operator=(const Client &rhs)
{
    _user = rhs._user;
    _user = rhs._nick;
    return (*this);
}
