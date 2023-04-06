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

    Client client = getClientByFd(client_fd);

    int pos = findChannelInArgs(inputClient);
    if (pos != 1)
    {
        std::cerr << "error finding channel\n";
        return (-1);
    }

    Client  *clientToKick = findClientByNick(inputClient[2]);
    if (!clientToKick)
    {
        sendNumericReplies(client_fd, ERR_NOSUCHNICK(getClientByFd(client_fd).getNickname(), inputClient[2]));
        return (1);
    }
    Channel *chan = findChannelByName(inputClient[pos]);

    if (findChannelByName(inputClient[pos])->isOperator(
        getClientByFd(client_fd).getNickname()) == 1)
    {
        std::string message = getClientByFd(client_fd).getNickname() + " KICK " + clientToKick->getNickname() + " " + chan->getChannelName() + "\r\n";
        chan->sendToAllClients(message , &client);
        chan->removeClientByFd(clientToKick->getFd());
    }
    else
    {
        sendNumericReplies(client_fd, ERR_CHANOPRIVSNEEDED(getClientByFd(client_fd).getNickname(), chan->getChannelName()));
    }
    
    return (0);
}
