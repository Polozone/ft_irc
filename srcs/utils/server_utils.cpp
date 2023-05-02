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
    const std::string s(str);
    if (s.find("\r\n") != std::string::npos)
        return (0);
    return (1);
}

struct pollfd createPollFdNode(int sd, int event)
{
    struct pollfd pollFdNode = {
        .fd = sd,
        .events = static_cast<short>(event),
        .revents = 0};

    return (pollFdNode);
}

int Server::isValidFd(int client_fd) const
{
    std::map<int, Client *>::const_iterator it = _clientsTryingToConnect.find(client_fd);
    if (it != _clients.end())
            return (client_fd);
   
    it = _clients.find(client_fd);
    if (it == _clients.end())
    {
        std::cerr << "Invalid Client fd\n";
        return (-1);
    }
    return (client_fd);
}

Client &Server::getClientByFd(int client_fd) const
{

    std::map<int, Client *>::const_iterator it = _clientsTryingToConnect.find(client_fd);
    if (it != _clientsTryingToConnect.end())
    {
        return (*it->second);
    }

    it = _clients.find(client_fd);
    if (it == _clients.end())
    {

        throw std::invalid_argument("Invalid client fd");
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
        delete _clientsTryingToConnect[client_fd];
        _clientsTryingToConnect.erase(client_fd);
        return (0);
    }
    
    it = _clients.find(client_fd);
    if (it != _clients.end())
    {
        delete _clients[client_fd];
        _clients.erase(client_fd);
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

/**
    @brief Frees the resources allocated for the server and deletes the server object.
    This function proceeds to call three member functions of the Server class to delete all 
    channels, all clients trying to connect, and all clients that are currently connected to 
    the server. Next, it frees the memory allocated for the servinfo structure, which was 
    obtained from the getaddrinfo() function. Finally, it deletes the server object itself.
    @param server A pointer to the Server object whose resources are to be freed.
    @return This function does not return anything.
*/

void Server::freeResources()
{
    deleteAllChannel();
    deleteAllClientsTryingToConnect();
    deleteAllClients();
    freeaddrinfo(servinfo);
}