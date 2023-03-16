
#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string channelName, std::string passwd) : _channelName(channelName), _passwd(passwd)
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