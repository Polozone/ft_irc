#include "../Server.hpp"

void Server::topicCommand(int client_fd, std::vector<std::string> inputClient)
{
    if (isValidFd(client_fd) == -1)
        return ;

    Client client = getClientByFd(client_fd);
    Channel *channel = findChannelByName(inputClient[1]);
    if (!channel)
    {
        sendNumericReplies(client_fd, ERR_NOSUCHCHANNEL(client.getNickname()));
        return ;
    }

    if (inputClient.size() == 2)
    {
        sendNumericReplies(client_fd, RPL_TOPIC(client.getNickname(), channel->getChannelName(), channel->getTopicContent()));
    }
    else if (inputClient.size() > 2)
    {
        if (channel->isOperator(client.getNickname()))
        {
            std::string newTopic = createStringWithParams(2, inputClient);
            newTopic = newTopic.substr(1, newTopic.size() - 1);
            channel->setTopicContent(newTopic);
        }
        else
        {
            sendNumericReplies(client_fd, ERR_CHANOPRIVSNEEDED(client.getNickname(), channel->getChannelName()));
        }
    }
}
