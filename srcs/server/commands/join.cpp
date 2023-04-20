#include "../Server.hpp"

void    Server::joinCommand(std::vector<std::string> command, int clientFd)
{
    std::vector<std::string> channelList;
    std::vector<std::string> passwdList;
    if (isValidFd(clientFd) == -1)
        return ;
    Client *client = _clients[clientFd];

    if (command[1].find(',') != std::string::npos)
        channelList = split(command[1], ',');
    else
        channelList.push_back(command[1]);

    if (command.size() > 2)
    {
        if (command[2].find(',') != std::string::npos)
            passwdList = split(command[2], ',');
        else
            passwdList.push_back(command[2]);
    }

    std::vector<std::string>::iterator it;
    size_t i = 0;

    for (it = channelList.begin(); it != channelList.end(); ++it)
    {
        Channel *channel;
        if ((channel = findChannelByName(channelList[i])) == NULL)
        {
            if (i < passwdList.size())
            {
                channel = new Channel(channelList[i], passwdList[i], client);
            }
            else
            {
                channel = new Channel(channelList[i], "", client);
            }
            addToChannelList(channel);
        }
        channelList[i] = channel->checkChannelName(channelList[i]);
        if (i < passwdList.size())
            channel->addClientToChannel(clientFd, client, passwdList[i]);
        else
            channel->addClientToChannel(clientFd, client, "");
        i++;
    }
}

void    Server::addToChannelList(Channel *toAdd)
{
    _channelList.push_back(toAdd);
}
