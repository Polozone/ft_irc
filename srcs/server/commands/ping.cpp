#include "../Server.hpp"

int Server::pingCommand(int client_fd, const std::string &buffer) const
{
    const std::string pong = "PONG o: " + extractCommandContent(buffer, "PING ") + "\r\n";

    if (send(client_fd, pong.data(), pong.size(), 0) < 0)
    {
        perror("send");
        return (-1);
    }
    return (0);
}