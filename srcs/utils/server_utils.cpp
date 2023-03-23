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
