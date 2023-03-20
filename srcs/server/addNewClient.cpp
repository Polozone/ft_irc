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
    size_t pos = buffer.find(command);
    size_t end;
    size_t i = 0;

    if (pos != std::string::npos)
    {
        end = pos + command.length();
        while (buffer[i + end] != 13 && buffer[i + end])
            i++;
    }

    return (buffer.substr(end, i));
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

int Server::checkIfNewClient(const char *buffer, int client_fd)
{
    std::string tmp(buffer);
    if (tmp.find("PASS ") != std::string::npos)
    {
        Client *newClient = new Client;
        newClient->setFd(client_fd);
        clientsTryingToConnect.push_back(newClient);
        addPassword(client_fd, extractCommandContent(tmp, "PASS "));
    }
    if (tmp.find("NICK ") != std::string::npos)
        addNick(client_fd, extractCommandContent(tmp, "NICK "));
    if (tmp.find("USER ") != std::string::npos)
    {
        addUser(client_fd, extractCommandContent(tmp, "USER "));
        handleConnection(client_fd);
    }
    return (0);
}

void Server::addNick(int client_fd, const std::string &nick)
{
    int i = findClientByFd(client_fd);
    clientsTryingToConnect[i]->setNickname(nick);
}

void Server::addUser(int client_fd, const std::string &user)
{
    int i = findClientByFd(client_fd);
    clientsTryingToConnect[i]->setUsername(user);
}

void Server::addPassword(int client_fd, const std::string &pass)
{
    int i = findClientByFd(client_fd);
    clientsTryingToConnect[i]->setPassword(pass);
}


int Server::handleConnection(int client_fd)
{
    const std::string tmp(password);
    int i = findClientByFd(client_fd);

    if (!tmp.compare(clientsTryingToConnect[i]->getPassword()))
    {
        return (welcomeClient(i, client_fd));
    }
    else
    {
        return (wrongPassword(i, client_fd));
    }
    return (0);
}

int Server::welcomeClient(int i, int client_fd)
{
    const std::string sPort(port);
    const std::string welcomeClient = ":localhost/" + sPort + " 001 " +
                                      clientsTryingToConnect[i]->getNickname() + " :Welcome to the server\r\n";
    if (send(client_fd, welcomeClient.data(), welcomeClient.size(), 0) < 0)
    {
        std::cerr << "Send error\n";
        return (-1);
    }
    clients.push_back(clientsTryingToConnect[i]);
    clientsTryingToConnect.erase(clientsTryingToConnect.begin() + i);
    return (0);
}

int Server::wrongPassword(int i, int client_fd)
{
    const std::string wrongPW = ":Wrong password, try again!\r\n";
    if (send(client_fd, wrongPW.data(), wrongPW.size(), 0) < 0)
    {
        std::cerr << "Send error\n";
        return (-1);
    }
    clientsTryingToConnect.erase(clientsTryingToConnect.begin() + i);
    return (1);
}
