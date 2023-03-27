#include "../server/Server.hpp"

void    Server::callCommand(std::vector<std::string> inputClient, const std::string rawClientInput, int clientFd)
{
    if (inputClient[0] == "JOIN")
        joinCommand(inputClient, clientFd);
    else if (inputClient[0] == "MODE")
        parseModeCommand(inputClient, clientFd);
    else if (inputClient[0] == "NICK")
        nickCommand(clientFd, inputClient[1]);
    else if (inputClient[0] == "PING")
        pingCommand(clientFd, rawClientInput);
    else if (inputClient[0] == "PRIVMSG")
        privmsgCommand(getClientByFd(clientFd), inputClient);
    else if (inputClient[0] == "PART")
        partCommand(clientFd, inputClient);
}

void    Server::setCommand(std::string &clientInput, int clientFd)
{
    std::vector<std::string> inputParsed;

    if (clientInput.empty())
        return ;

    std::string withoutExtraSpace = removeExtraSpaces(clientInput);
    inputParsed = split(withoutExtraSpace, ' ');

    if (inputParsed.size() < 2)
    {
        sendNumericReplies(clientFd, ERR_NEEDMOREPARAMS(inputParsed[0]));
        return ;
    }
    callCommand(inputParsed, clientInput, clientFd);
}