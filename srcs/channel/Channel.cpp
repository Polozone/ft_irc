#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string channelName, std::string passwd, Client *creator)
    : _channelName(channelName), _passwd(passwd), _creator(creator),
    _isPrivate(false), _isSecret(false), _isInviteOnly(false),
    _topic(""), _maxClients(100)
{}

void Channel::removeOperator(std::string &opName)
{
    size_t  index = 0;

    for (_it = _operators.begin(); _it != _operators.end(); ++_it)
        if (*_it == opName)
            break ;
    if (_it != _operators.end())
        _operators.erase(_operators.begin() + index);
}

void Channel::addOperator(std::string &opName)
{
    _operators.push_back(opName);
}

void    Channel::printOperatos()
{
    for (_it = _operators.begin(); _it != _operators.end(); ++_it)
        std::cout << *_it << std::endl;
}

void    Channel::addClientToChannel(int fdClient, Client *clientToAdd)
{
    if (! _nbrClientsConnected > _maxClients)
    {
        _clients.insert(std::make_pair(fdClient, clientToAdd));
        _nbrClientsConnected++;
    }
}

void    Channel::removeClientByFd(int fdClient)
{
    _clients.erase(fdClient);
    _nbrClientsConnected--;
}