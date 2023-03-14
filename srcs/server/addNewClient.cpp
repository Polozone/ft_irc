#include "./Server.hpp"

int Server::addNewClient(const char *buffer, int client_fd)
{
    if (checkIfClient(buffer) == 1)
        getCommandContent(buffer);
    else if (checkIfClient(buffer) == 2)
        getCommandContent(buffer);
    
    std::string to_send(":localhost 001 tdeville :Welcome to IRC Network, tdeville!tdeville@tdeville\r\n");
    if (send(client_fd, to_send.data(), to_send.size(), 0) < 0)
        printf("send error\n");
    return (0);
}

int Server::checkIfClient(const char *buffer) const
{
    const std::string tmp(buffer);

    if (tmp.find("NICK ") != std::string::npos) {
        return (1);
    }
    else if (tmp.find("USER ") != std::string::npos) {
        return (2);
    }
    return (0);
}

std::string getCommandContent(const char *buffer)
{
    std::string commandContent;
    printf("11\n");
    std::string tmp(buffer);
    printf("22\n");
    int i = 0;

    while (buffer[i] != 32 && buffer[i])
        i++;
    
    i++;
    printf("33\n");
    commandContent = tmp.substr(i, tmp.size() - i);
    printf("44\n");

    std::cout << commandContent << std::endl;

    return NULL;
}
