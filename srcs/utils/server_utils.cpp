#include "../server/Server.hpp"

int handleServerErrors(const char *str, int *sd)
{
    perror(str);
    if (*sd)
    {
        close(*sd);
    }
    return (1);
}

int detectEOF(const char *str)
{
    int i = 0;
    while (str[i])
    {
        if (!str[i + 1] && str[i] != '\n')
            return (1);
        i++;
    }
    return (0);
}

struct pollfd createPollFdNode(int sd, int event)
{
    struct pollfd pollFdNode = {
        .fd = sd,
        .events = event};

    return (pollFdNode);
}

int Server::isValidFd(int client_fd) const
{
    try
    {
        std::map<int, Client *>::const_iterator it = _clientsTryingToConnect.find(client_fd);
        if (it != _clients.end())
            return (client_fd);
    }
    catch(const std::exception& e)
    {
    }
    
    try
    {
        std::map<int, Client *>::const_iterator it = _clients.find(client_fd);
        if (it == _clients.end())
            throw std::invalid_argument("Invalid client fd");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error finding client: " << e.what() << '\n';
        return (-1);
    }
    return (client_fd);
}

Client &Server::getClientByFd(int client_fd) const
{
    try
    {
        std::map<int, Client *>::const_iterator it = _clientsTryingToConnect.find(client_fd);
        if (it != _clientsTryingToConnect.end())
        {
            return (*it->second);
        }
    }
    catch (const std::exception &e)
    {
    }
    std::map<int, Client *>::const_iterator it;
    try
    {
        it = _clients.find(client_fd);
        if (it == _clients.end())
            throw std::invalid_argument("Invalid client fd");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error finding client: " << e.what() << '\n';
    }
    return (*it->second);
}

void    sendNumericReplies(int fd, const std::string &message)
{
    if (send(fd, message.data(), message.size(), 0) == -1){
        perror("send() failed");
        return ;
    }
}

int Server::removeClientFromMap(int client_fd)
{
    std::map<int, Client *>::iterator it;

    it = _clientsTryingToConnect.find(client_fd);
    if (it != _clientsTryingToConnect.end())
    {
        _clientsTryingToConnect.erase(it);
        return (0);
    }
    
    it = _clients.find(client_fd);
    if (it != _clients.end())
    {
        _clients.erase(it);
        return (0);
    }
    std::cerr << "error: Client to erase not found\n";
    return (-1);
}


// DEBUG FUNCTIONS
void Server::printClientMaps() const
{
    std::map<int, Client *>::const_iterator it;
    int i = 0;

    std::cout << "_clients - Size: " << _clients.size() << "\n";
    for(it = _clients.begin(); it != _clients.end(); ++it)
    {
        std::cout << "[" << i << "]" << " fd: " << it->first << "\n";
        std::cout << " NICK: " << it->second->getNickname() << "\n";
        std::cout << " USER: " << it->second->getUsername() << "\n";
        std::cout << " PASS: " << it->second->getPassword() << "\n";
        std::cout << "\n";
        i++;
    }
    std::cout << "_clientsTryingToConnect - Size: " << _clientsTryingToConnect.size() << "\n";
    for(it = _clientsTryingToConnect.begin(); it != _clientsTryingToConnect.end(); ++it)
    {
        std::cout << "[" << i << "]" << " fd: " << it->first << "\n";
        std::cout << " NICK: " << it->second->getNickname() << "\n";
        std::cout << " USER: " << it->second->getUsername() << "\n";
        std::cout << " PASS: " << it->second->getPassword() << "\n";
        std::cout << "\n";
        i++;
    }
}
