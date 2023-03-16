#include "./Server.hpp"

static int commandNumber(const char *buffer)
{
    int i = 0;
    int cmd = 0;

    while (buffer[i])
    {
        if (buffer[i] == 13 && buffer[i + 1] == 10)
            cmd++;
        i++;
    }
    return (cmd);
}

const std::string extractCommandContent(const std::string &buffer, const std::string &command)
{
    size_t  pos = buffer.find(command);
    size_t  end;

    if (pos != std::string::npos)
        end = pos + command.length();

    return (buffer.substr(end, buffer.length() - end));
}

int Server::findClientByFd(int client_fd) const
{
    for (int i = 0; i < clientsTryingToConnect.size(); i++)
    {
        if (clientsTryingToConnect[i]->getFd() == client_fd)
            return (i);
    }
    std::cerr << "Client Not Found By Fd\n";
    return (-1);
}

int askPassword(int client_fd)
{
    printf("ici meme\n");
    std::string rplPasswd(":localhost 001 :Welcome to the localhost/4242 Network, tdeville!theodeville@theodeville");
    if (send(client_fd, rplPasswd.data(), rplPasswd.size(), 0) < 0)
        printf("Send error\n");
    return (0);
}

int Server::checkIfClient(const char *buffer, int client_fd)
{
    std::string tmp(buffer);
    if (tmp.find("CAP LS ") != std::string::npos)
    {
        Client *newClient = new Client;
        newClient->setFd(client_fd);
        clientsTryingToConnect.push_back(newClient);
    }
    if (tmp.find("NICK ") != std::string::npos)
        addNick(client_fd, extractCommandContent(tmp, "NICK "));
    if (tmp.find("USER ") != std::string::npos)
    {
        addUser(client_fd, extractCommandContent(tmp, "USER "));
        askPassword(client_fd);
    }
    return (0);
}

void    Server::addNick(int client_fd, const std::string &nick)
{
    int i = findClientByFd(client_fd);
    clientsTryingToConnect[i]->setNick(nick);
}

void    Server::addUser(int client_fd, const std::string &user)
{
    int i = findClientByFd(client_fd);
    clientsTryingToConnect[i]->setUser(user);
}

