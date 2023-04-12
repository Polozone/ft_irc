
#include "../Server.hpp"

void Server::killCommand(int clientFd, const std::string &clientName)
{
    Client *caller = findClientByNick(getClientByFd(clientFd).getNickname());

    if ( ! caller->getOperatorStatus() )
    {
        caller->sendMessage(ERR_NOPRIVILEGES);
        return ;
    }

    Client *clientToKill = findClientByNick(clientName);

    if (clientToKill == NULL)
    {
        caller->sendMessage(ERR_NOSUCHNICK(caller->getNickname(), clientName));
        return ;
    }

    closeConnectionByFd(clientToKill->getFd());
}
