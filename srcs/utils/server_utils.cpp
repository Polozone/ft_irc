#include "../server/Server.hpp"

int handleServerErrors(const char *str, int *sd) {
    perror(str);
    if (*sd) {
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

int Server::findConnectedClientByFd(int client_fd)
{
    try
    {
        std::map<int , Client *>::iterator it = clients.find(client_fd); 
        if (it == clients.end())
            throw std::invalid_argument("Invalid client fd");
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error finding client: " << e.what() << '\n';
        return (-1);
    }
    return (client_fd);
}
