
#include "../Server.hpp"

static int    parseFlags(std::string &flags)
{
    std::string::iterator it;
    int counter = 0;
    for (size_t i = 0; i < flags.length(); i++)
    {
        if (flags[i] != '-' && flags[i] != '+' && flags[i] != 'p' 
            && flags[i] != 's' && flags[i] != 'i' && flags[i] != 'm' 
            && flags[i] != 'v' && flags[i] != 't' && flags[i] != 'l' && flags[i] != 'o')
        {
            return (-1);
        }
        if (flags[i] == 't' || flags[i] == 'l' || flags[i] == 'o' || flags[i] == 'v')
            counter++;
    }
    return (counter);
}

void    Server::modeLflag(char sign, Channel *targetedChannel, std::string limitString)
{
    int limit;

    std::stringstream ss(limitString);
    ss >> limit;
    if ( ! limitString.empty() && isDigits(limitString))
    {
        if (sign == '+' || sign == '-')
            targetedChannel->setMaxClient(limit);
    }
}

void    Server::modeOflag(char sign, Channel *targetedChannel, std::string nameClientTargeted, Client *caller)
{
    std::string message;
    Client *clientTargeted;

    if ( ! nameClientTargeted.empty() )
        clientTargeted = findClientByNick(nameClientTargeted);
    else
        return ;

    if (sign == '+')
    {
        message = ": MODE " + targetedChannel->getChannelName() + " +o " + nameClientTargeted;
        targetedChannel->addOperator(clientTargeted);
        targetedChannel->sendToAllClients(message, caller);
    }
    else if (sign == '-')
    {
        message = ": MODE " + targetedChannel->getChannelName() + " -o " + nameClientTargeted;
        targetedChannel->removeOperator(clientTargeted->getFd());
        targetedChannel->sendToAllClients(message, caller);
    }
}


void    Server::modeTflag(char sign, Channel *targetedChannel)
{
    if (sign == '+')
        targetedChannel->setTopicStatus(true);
    else if (sign == '-')
        targetedChannel->setTopicStatus(false);
}

void    Server::modeSflag(char sign, Channel *targetedChannel)
{
    if (sign == '+')
        targetedChannel->setSecretStatus(true);
    else if (sign == '-')
        targetedChannel->setSecretStatus(false);
}

void    Server::modeMflag(char sign, Channel *targetedChannel)
{
    if (sign == '+')
    {
        targetedChannel->setStatusModerate(true);
    }
    else if (sign == '-')
    {
        targetedChannel->setStatusModerate(false);
    }
}

void    Server::modeVflag(char sign, Channel *targetedChannel, std::string clientName)
{
    Client *targetedClient = findClientByNick(clientName);
    if (sign == '+')
    {
        targetedChannel->addClientToSpeakList(targetedClient);
    }
    else if (sign == '-')
        targetedChannel->rmvClientFromSpeakList(targetedClient->getFd());
}

void    Server::modePflag(char sign, Channel *targetedChannel)
{
    if (sign == '+')
        targetedChannel->setPrivateStatus(true);
    else if (sign == '-')
        targetedChannel->setPrivateStatus(false);
}

void    Server::modeIflag(char sign, Channel *targetedChannel)
{
    if (sign == '+')
        targetedChannel->setInviteStatus(true);
    else if (sign == '-')
        targetedChannel->setInviteStatus(false);
}

void    Server::executeFlags(std::vector<std::string> command, int clientFd, Channel *targetedChannel)
{
    std::string flags = command[2];
    size_t counter = 3;
    std::string actualArg;
    Client caller = getClientByFd(clientFd);

    for (int i = 0; i < static_cast<int>(flags.size()); i++)
    {
        if (counter < command.size())
            actualArg = command[counter];
        else
            actualArg = "";

        if (i - 1 < 0)
            i++;
        if (flags[i] && flags[i] == 'o')
        {
            modeOflag(flags[i - 1], targetedChannel, actualArg, &caller);
            counter++;
        }
        else if (flags[i] && flags[i] == 'l')
        {
            modeLflag(flags[i - 1], targetedChannel, actualArg);
            counter++;
        }
        else if (flags[i] && flags[i] == 't')
        {
            modeTflag(flags[i - 1], targetedChannel);
            counter++;
        }
        else if (flags[i] && flags[i] == 'v')
        {
            modeVflag(flags[i - 1], targetedChannel, actualArg);
            counter++;
        }
        else if (flags[i] && flags[i] == 'm')
            modeMflag(flags[i - 1], targetedChannel);
        else if (flags[i] && flags[i] == 'p')
            modePflag(flags[i - 1], targetedChannel);
        else if (flags[i] && flags[i] == 's')
            modePflag(flags[i - 1], targetedChannel);
        else if (flags[i] && flags[i] == 'i')
            modeIflag(flags[i - 1], targetedChannel);
    }
}

void    Server::parseChannelModeCommand(std::vector<std::string> command, int clientFd)
{
    std::string targetChannelName;
    Channel *targetedChannel;
    int flagNeedArgs = 0;

    if (isValidFd(clientFd) == -1)
        return ;

    Client client = getClientByFd(clientFd);

    if (command.size() > 1)
        targetChannelName = command[1];

    if ((targetedChannel = findChannelByName(targetChannelName)) == NULL)
    {
        return ;
    }

    if (targetedChannel->isOperator(client.getNickname()) == false)
    {
        client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), targetedChannel->getChannelName()));
        return ;
    }

    if (command.size() > 2)
    {
        if ((flagNeedArgs = parseFlags(command[2])) == -1)
            return ;
        executeFlags(command, clientFd, targetedChannel);
    }
}