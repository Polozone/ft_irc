#include "../Server.hpp"

int findChannelInArgs(std::vector<std::string> args)
{
    for (size_t i = 0; i < args.size(); i++)
    {
        if (args[i].at(0) == '#')
            return (i);
    }
    return (-1);
}

int Server::kickCommand(int client_fd, std::vector<std::string> inputClient)
{
    if (isValidFd(client_fd) == -1)
        return (-1);

    int pos = findChannelInArgs(inputClient);
    if (pos != 1)
    {
        std::cerr << "error finding channel\n";
        return (-1);
    }

    Client  *clientToKick = findClientByNick(inputClient[2]);
    std::cout << inputClient[pos] << std::endl;
    Channel *chan = findChannelByName(inputClient[pos]);

    if (findChannelByName(inputClient[pos])->isOperator(
        getClientByFd(client_fd).getNickname()) == 1)
    {
        std::cout << "KICK " + clientToKick->getNickname() + " " + chan->getChannelName() + "\r\n" << std::endl;
        chan->sendToAllClients(getClientByFd(client_fd).getNickname() + " KICK " + clientToKick->getNickname() + " " + chan->getChannelName() + "\r\n", getClientByFd(client_fd));
        chan->removeClientByFd(clientToKick->getFd());
    }
    
    return (0);
}
