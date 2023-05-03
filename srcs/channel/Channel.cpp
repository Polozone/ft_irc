#include "Channel.hpp"

Channel::Channel(){}

Channel::~Channel(){}

Channel::Channel(std::string channelName, std::string passwd, Client *creator)
    : _channelName(checkChannelName(channelName)), _passwd(passwd), _creator(creator),
    _isPasswd(false), _nbrClientsConnected(0), _isPrivate(false), _isSecret(false), 
    _isInviteOnly(false), _topic("default topic"), _maxClients(1000), _isModerate(false), _isVoice(false)
{
    addClientToChannel(creator->getFd(), creator, passwd);
    addOperator(creator);
    if (_passwd != "")
        _isPasswd = true;
}

std::string Channel::checkChannelName(std::string &channelName)
{
    if (channelName[0] != '#')
        channelName.insert(0, "#");
    return (channelName);
}

void Channel::removeOperator(int fdClient)
{
    _operators.erase(fdClient);
    rmvClientFromSpeakList(fdClient);
}

void Channel::addOperator(Client *clientToAdd)
{
    std::string clientName = clientToAdd->getNickname();

    if (isClientExist(clientName) && ! isOperator(clientToAdd->getNickname()))
    {
        std::pair<std::map<int, Client*>::iterator, bool> result = _operators.insert(std::make_pair(clientToAdd->getFd(), clientToAdd));
        if(result.second) {
            addClientToSpeakList(clientToAdd);
        }
    }
    printOperators();
}

void    Channel::printOperators()
{
    for (_itm = _operators.begin(); _itm != _operators.end(); ++_itm)
        std::cout << (*_itm).second->getNickname() << std::endl;
}

bool    Channel::checkPasswd(const std::string& passwd, Client * clientToAdd)
{
    if (_isPasswd && _passwd != passwd)
    {
        clientToAdd->sendMessage(ERR_WRONGPSSWD(_channelName));
        return false;
    }
    return true;
}

void    Channel::addClientToChannel(int fdClient, Client *clientToAdd, const std::string& passwd)
{
    if ( ! checkPasswd(passwd, clientToAdd))
        return ;
    if (_isInviteOnly)
        clientToAdd->sendMessage(ERR_INVITEONLYCHAN(clientToAdd->getNickname()));
    else
    {
        if ( _nbrClientsConnected < _maxClients)
        {
            std::pair<std::map<int, Client*>::iterator, bool> result = _clients.insert(std::make_pair(fdClient, clientToAdd));
            if(result.second) {
                clientToAdd->sendMessage(RPL_NAMREPLY(clientToAdd->getUsername(), _channelName, clientToAdd->getNickname()));
                std::string message = ":" + clientToAdd->getNickname() + " JOIN " + _channelName + "\r\n";
                sendToAllClients(message, clientToAdd);
                _nbrClientsConnected++;
            }
        }
        else
        {
            clientToAdd->sendMessage(ERR_CHANNELISFULL(_channelName));
        }
    }
}

void Channel::addClientToChannelInvite(int fdClientToAdd, Client *clientToAdd)
{
    if (_nbrClientsConnected < _maxClients)
    {
        std::pair<std::map<int, Client *>::iterator, bool> result = _clients.insert(std::make_pair(fdClientToAdd, clientToAdd));
        if (result.second)
        {
            clientToAdd->sendMessage(RPL_NAMREPLY(clientToAdd->getUsername(), _channelName, clientToAdd->getNickname()));
            std::string message = ":" + clientToAdd->getNickname() + " JOIN " + _channelName + "\r\n";
            sendToAllClients(message, clientToAdd);
            _nbrClientsConnected++;
        }
    }
    else
    {
        clientToAdd->sendMessage(ERR_CHANNELISFULL(_channelName));
    }
}

void    Channel::removeClientByFd(int fdClient)
{
    _clients.erase(fdClient);
    _nbrClientsConnected--;
}

void    Channel::addClientToSpeakList(Client *clientToAdd)
{
    std::string clientName = clientToAdd->getNickname();
    
    if (isClientExist(clientName))
    {
        _canSpeakList.insert(std::make_pair(clientToAdd->getFd(), clientToAdd));
    }
}

void    Channel::rmvClientFromSpeakList(int clientFd)
{
    _canSpeakList.erase(clientFd);
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
    for (_itm = _canSpeakList.begin(); _itm != _canSpeakList.end(); ++_itm)
        std::cout << (*_itm).second->getNickname() << " ";
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

void Channel::sendToAllClients(std::string &message, Client *sender)
{
    if (_isModerate)
    {
        if ( isSpeakList(*sender) )
            sendMsgToSpeakList(message, *sender);
        else
            return ;
    }
    for (_itm = _clients.begin(); _itm != _clients.end(); ++_itm)
    {
        (*_itm).second->sendMessage(message);
    }
}

bool    Channel::isOperator(const std::string &clientName)
{
    for (_itm = _operators.begin(); _itm != _operators.end(); ++_itm)
    {
        if ((*_itm).second->getNickname() == clientName)
            return (true);
    }
    return (false);
}
// This function sends a message to all clients connected to a channel except for the one specified.
void Channel::sendToChannel(const std::string &message, const Client &user)
{
    if (_isModerate)
    {
        if ( isSpeakList(user) )
        {
            sendMsgToSpeakList(message, user);
            return ;
        }
        else
            return ;
    }
    // Iterate over all clients connected to the channel
    for (_itm = _clients.begin(); _itm != _clients.end(); ++_itm)
    {
        // If the current client being iterated is not the one specified
        if (user.getFd() != (*_itm).second->getFd())
        {
            // Send the message to the current client
            (*_itm).second->sendMessage(message);
        }
    }
}

void    Channel::sendMsgToSpeakList(const std::string &message, const Client &sender)
{
    for (_itm = _canSpeakList.begin(); _itm != _canSpeakList.end(); ++_itm)
    {
        if ((*_itm).second->getNickname() != sender.getNickname())
            (*_itm).second->sendMessage(message);
    }
}

bool    Channel::isSpeakList(const Client &client)
{
    for (_itm = _canSpeakList.begin(); _itm != _canSpeakList.end(); ++_itm)
    {
        if ((*_itm).second->getNickname() == client.getNickname())
        {
            return (true);
        }
    }
    return (false);
}

