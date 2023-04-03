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

    if (isValidFd(clientFd) == -1)
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

/**
    @brief This function adds a new operator to the Server object with the given username \ 
    and password.
    @param user A string representing the username of the new operator.
    @param password A string representing the password of the new operator.
    @throws std::runtime_error If the given username already exists in the map of operator \ 
    credentials.
    @note This function does not return anything; instead, it throws an exception if the \ 
    given username already exists.
*/
void Server::addOperCreds(std::string user, std::string password)
{
    try
    {
        std::map<std::string, std::string>::iterator it = this->\
        _operatorCredentials.find(user);

        if (it != this->_operatorCredentials.end())
        {
            throw std::runtime_error("Credentials for this user already established");
        }
        this->_operatorCredentials[user] = password;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

/**
    @brief This function checks if the given username and password match the operator \
    stored in the Server object.
    @param username A constant reference to a string representing the username of the operator.
    @param password A constant reference to a string representing the password of the operator.
    @return A boolean value indicating if the provided credentials match or not.
*/
bool Server::checkOperCreds(const std::string &username, const std::string &password) const
{
    std::map<std::string, std::string>::const_iterator it = _operatorCredentials.find(username);

    if (it == _operatorCredentials.end())
        return false; // Username not found

    return  (password == it->second);
}