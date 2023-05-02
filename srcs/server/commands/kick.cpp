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

static std::string createStringWithParams(std::vector<std::string> inputClient)
{
    std::string str(": ");
    for (size_t i = 3; i < inputClient.size(); i++)
    {
        str += inputClient.at(i);
        if (i + 1 != inputClient.size())
            str += " ";
    }
    return (str);
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
        sendNumericReplies(client_fd, ERR_NOSUCHNICK(client.getNickname(), inputClient[2]));
        return (1);
    }
    Channel *chan = findChannelByName(inputClient[pos]);

    if (findChannelByName(inputClient[pos])->isOperator(
        client.getNickname()) == 1)
    {
        std::string reason(" :You've been kicked\r\n");
        try
        {
            if (!inputClient.at(3).empty())
                reason = createStringWithParams(inputClient);
        }
        catch(const std::exception& e)
        {
        }
        
        std::string message = ":" + client.getNickname() + " KICK " + chan->getChannelName() + " " + clientToKick->getNickname() + reason;
        chan->sendToAllClients(message , &client);
        chan->removeClientByFd(clientToKick->getFd());
    }
    else
    {
        sendNumericReplies(client_fd, ERR_CHANOPRIVSNEEDED(client.getNickname(), chan->getChannelName()));
    }
    
    return (0);
}
