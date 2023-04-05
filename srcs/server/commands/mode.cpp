
#include "../Server.hpp"

static int    parseFlags(std::string &flags)
{
    std::string::iterator it;
    int counter = 0;
    for (int i = 0; i < flags.length(); i++)
    {
        if (flags[i] != '-' && flags[i] != '+' && flags[i] != 'p' 
            && flags[i] != 's' && flags[i] != 'i' && flags[i] != 'm' 
            && flags[i] != 'v' && flags[i] != 't' && flags[i] != 'l' && flags[i] != 'o')
        {
            std::cout << "MODE/ #Channel -+psitlmvo [args...]" << std::endl;
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
        else
            std::cout << "bad format, except + or - before flag" << std::endl;
    }
    else
    {
        std::cout << "flag -l: invalid argument" << std::endl;
    }
    std::cout << "limit of " << targetedChannel->getChannelName() << " is " << targetedChannel->getMaxClient() << std::endl;
}

void    Server::modeOflag(char sign, Channel *targetedChannel, std::string clientTargeted)
{
    std::string message;

    if ( ! clientTargeted.empty() )
    {
        if (sign == '+')
        {
            message = ": MODE " + targetedChannel->getChannelName() + " +o " + clientTargeted;
            targetedChannel->addOperator(clientTargeted);
            targetedChannel->sendToAllClients(message);
        }
        else if (sign == '-')
        {
            message = ": MODE " + targetedChannel->getChannelName() + " -o " + clientTargeted;
            targetedChannel->removeOperator(clientTargeted);
            targetedChannel->sendToAllClients(message);
        }
        else
            std::cout << "bad format, except + or - before flag" << std::endl;
    }
    targetedChannel->printOperators();
}

void    Server::modeTflag(char sign, Channel *targetedChannel, std::string clientTargeted)
{
    if (sign == '+')
        targetedChannel->setTopicStatus(true);
    else if (sign == '-')
        targetedChannel->setTopicStatus(false);
    else
        std::cout << "bad format, except + or - before flag" << std::endl;
}

void    Server::modeSflag(char sign, Channel *targetedChannel, std::string clientTargeted)
{
    if (sign == '+')
        targetedChannel->setSecretStatus(true);
    else if (sign == '-')
        targetedChannel->setSecretStatus(false);
    else
        std::cout << "bad format, except + or - before flag" << std::endl;
}

void    Server::modeMflag(char sign, Channel *targetedChannel, std::string clientTargeted)
{
    if (sign == '+')
        targetedChannel->setStatusModerate(true);
    else if (sign == '-')
        targetedChannel->setStatusModerate(false);
    else
        std::cout << "bad format, except + or - before flag" << std::endl;
}

void    Server::modeVflag(char sign, Channel *targetedChannel, std::string clientTargeted)
{
    if (sign == '+')
        targetedChannel->addClientToSpeakList(clientTargeted);
    else if (sign == '-')
        targetedChannel->rmvClientFromSpeakList(clientTargeted);
    else
        std::cout << "bad format, except + or - before flag" << std::endl;
}

void    Server::modePflag(char sign, Channel *targetedChannel, std::string clientTargeted)
{
    if (sign == '+')
        targetedChannel->setPrivateStatus(true);
    else if (sign == '-')
        targetedChannel->setPrivateStatus(false);
    else
        std::cout << "bad format, except + or - before flag" << std::endl;
}

void    Server::modeIflag(char sign, Channel *targetedChannel, std::string clientTargeted)
{
    if (sign == '+')
        targetedChannel->setInviteStatus(true);
    else if (sign == '-')
        targetedChannel->setInviteStatus(false);
    else
        std::cout << "bad format, except + or - before flag" << std::endl;
}

void    Server::executeFlags(int flagNeedArgs, std::vector<std::string> command, int clientFd, Channel *targetedChannel)
{
    std::string flags = command[2];
    int counter = 3;
    std::string actualArg;

    for (int i = 0; i < flags.size(); i++)
    {
        if (counter < command.size())
            actualArg = command[counter];
        else
            actualArg = "";

        if (i - 1 < 0)
            i++;
        if (flags[i] && flags[i] == 'o')
        {
            modeOflag(flags[i - 1], targetedChannel, actualArg);
            counter++;
        }
        else if (flags[i] && flags[i] == 'l')
        {
            modeLflag(flags[i - 1], targetedChannel, actualArg);
            counter++;
        }
        else if (flags[i] && flags[i] == 't')
        {
            modeTflag(flags[i - 1], targetedChannel, actualArg);
            counter++;
        }
        else if (flags[i] && flags[i] == 'v')
        {
            modeVflag(flags[i - 1], targetedChannel, actualArg);
            counter++;
        }
        else if (flags[i] && flags[i] == 'm')
            modeSflag(flags[i - 1], targetedChannel, actualArg);
        else if (flags[i] && flags[i] == 'p')
            modePflag(flags[i - 1], targetedChannel, actualArg);
        else if (flags[i] && flags[i] == 's')
            modePflag(flags[i - 1], targetedChannel, actualArg);
        else if (flags[i] && flags[i] == 'i')
            modeIflag(flags[i - 1], targetedChannel, actualArg);
    }
}


void    Server::parseChannelModeCommand(std::vector<std::string> command, int clientFd)
{
    std::string targetChannelName;
    Channel *targetedChannel;
    Client client = getClientByFd(clientFd);
    int flagNeedArgs = 0;

    if (command.size() > 1)
        targetChannelName = command[1];

    if ((targetedChannel = findChannelByName(targetChannelName)) == NULL)
    {
        std::cout << "Channel does not exist" << std::endl;
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
        executeFlags(flagNeedArgs, command, clientFd, targetedChannel);
    }
}