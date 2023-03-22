#include "../server/Server.hpp"

void    Server::callCommand(std::vector<std::string> inputClient, int clientFd)
{
    if (inputClient[0] == "JOIN")
        joinCommand(inputClient, clientFd);
    else if (inputClient[0] == "MODE")
        parseModeCommand(inputClient, clientFd);
}

void    Server::setCommand(std::string &clientInput, int clientFd)
{
    std::vector<std::string> inputParsed;
    if (clientInput.empty())
        return ;
    std::string withoutExtraSpace = removeExtraSpaces(clientInput);
    inputParsed = split(withoutExtraSpace, ' ');
    if (inputParsed.size() < 2)
        return ;
    callCommand(inputParsed, clientFd);
}