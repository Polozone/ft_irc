
#include "../Server.hpp"

void Server::killCommand(int clientFd, const std::string &clientName)
{
    Client caller = getClientByFd(clientFd);

    if ( ! caller.getOperatorStatus() )
    {
        caller.sendMessage(ERR_NOPRIVILEGES);
        return ;
    }

    Client *clientToKill = findClientByNick(clientName);

    if (clientToKill == NULL)
    {
        caller.sendMessage(ERR_NOSUCHNICK(caller.getNickname(), clientName));
        return ;
    }

    closeConnectionByFd(clientToKill->getFd());
}