#include "Channel.hpp"

Channel::Channel(){}

Channel::Channel(std::string channelName, std::string passwd, Client *creator)
    : _channelName(channelName), _passwd(passwd), _creator(creator),
    _isPrivate(false), _isSecret(false), _isInviteOnly(false),
    _topic("default topic"), _maxClients(1000), _isModerate(false), _isVoice(false)
{
    addClientToChannel(creator->getFd(), creator);
    addOperator(creator->getNickname());
}

void Channel::removeOperator(std::string &opName)
{
    size_t  index = 0;

    for (_it = _operators.begin(); _it != _operators.end(); ++_it)
        if (*_it == opName)
            break ;
    if (_it != _operators.end())
        _operators.erase(_operators.begin() + index);
}

void Channel::addOperator(std::string opName)
{
    if (isClientExist(opName))
        _operators.push_back(opName);
}

void    Channel::printOperators()
{
    for (_it = _operators.begin(); _it != _operators.end(); ++_it)
        std::cout << *_it << std::endl;
}

void    Channel::addClientToChannel(int fdClient, Client *clientToAdd)
{
    if (_isInviteOnly)
        sendNumericReplies(fdClient, ERR_INVITEONLYCHAN(clientToAdd->getNickname()));
    else
    {
        if ( _nbrClientsConnected < _maxClients)
        {
            _clients.insert(std::make_pair(fdClient, clientToAdd));
            sendNumericReplies(fdClient, RPL_TOPIC(_channelName, _topicContent));
            sendNumericReplies(fdClient, RPL_NAMREPLY(clientToAdd->getUsername(), _channelName, clientToAdd->getNickname()));
            std::string message = ":" + clientToAdd->getNickname() + " JOIN " + _channelName + "\r\n";
            sendToAllClients(message);
            _nbrClientsConnected++;
        }
        else
        {
            sendNumericReplies(fdClient, ERR_CHANNELISFULL(_channelName));
        }
    }
}

void    Channel::removeClientByFd(int fdClient)
{
    _clients.erase(fdClient);
    _nbrClientsConnected--;
}

void    Channel::addClientToSpeakList(std::string &clientName)
{
    _canSpeakList.push_back(clientName);
}

void    Channel::rmvClientFromSpeakList(std::string &clientName)
{
    size_t  index = 0;

    for (_it = _canSpeakList.begin(); _it != _canSpeakList.end(); ++_it)
        if (*_it == clientName)
            break ;
    if (_it != _canSpeakList.end())
        _canSpeakList.erase(_canSpeakList.begin() + index);
}

bool    Channel::isClientExist(std::string &clientName)
{
    for (_itm = _clients.begin(); _itm != _clients.end(); ++_itm)
    {
        if ((*_itm).second->getNickname() == clientName)
            return (true);
    }
    return (false);
}

void    Channel::printClientList()
{
    std::cout << "Client list: ";
    for (_itm = _clients.begin(); _itm != _clients.end(); ++_itm)
        std::cout  << (_itm)->second->getNickname() << " ";
    std::cout << std::endl;
}

void    Channel::printSpeakList()
{
    for (_it = _canSpeakList.begin(); _it != _canSpeakList.end(); ++_it)
        std::cout << *_it << " ";
    std::cout << std::endl;
}

Client * Channel::findClientByFd(int fd)
{
    _itm = _clients.find(fd);
    if (_itm != _clients.end())
        return _itm->second;
    return (NULL);
}

void    Channel::addInvitedClient(std::string toAdd)
{
    _invitedClient.push_back(toAdd);
}

bool    Channel::isClientIsInvited(std::string &clientName)
{
    for (_it = _invitedClient.begin(); _it != _invitedClient.end(); ++_it)
    {
        if (*_it == clientName)
            return (true);
    }
    return (false);
}

void Channel::sendToAllClients(std::string &message)
{
    for (_itm = _clients.begin(); _itm != _clients.end(); ++_itm)
    {
        sendNumericReplies((*_itm).second->getFd(), message);
    }
}
