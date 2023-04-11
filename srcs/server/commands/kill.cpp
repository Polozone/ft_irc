
#include "../Server.hpp"

void Server::killCommand(int clientFd, const std::string &clientName)
{
    Client *caller = findClientByNick(getClientByFd(clientFd).getNickname());

    if ( ! caller->getOperatorStatus() )
    {
        std::cout << "test1.5" << std::endl;
        caller->sendMessage(ERR_NOPRIVILEGES);
        return ;
    }
    std::cout << "test2" << std::endl;

    Client *clientToKill = findClientByNick(clientName);

    if (clientToKill == NULL)
    {
        std::cout << "test3" << std::endl;
        caller->sendMessage(ERR_NOSUCHNICK(caller->getNickname(), clientName));
        return ;
    }
    std::cout << "test4" << std::endl;

    closeConnectionByFd(clientToKill->getFd());
}
