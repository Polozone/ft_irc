#include "./Server.hpp"

static int commandNumber(const char *buffer)
{
    int i = 0;
    int cmd = 0;

    while (buffer[i])
    {
        if (buffer[i] == 13 && buffer[i + 1] == 10)
            cmd++;
        i++;
    }
    return (cmd);
}

const std::string extractCommandContent(const std::string &buffer, const std::string &command)
{
    size_t pos = buffer.find(command);
    size_t end;
    size_t i = 0;

    if (pos != std::string::npos)
    {
        end = pos + command.length();
        while (buffer[i + end] != 13 && buffer[i + end])
            i++;
    }

    return (buffer.substr(end, i));
}

int Server::findClientByFd(int client_fd) const
{
    try
    {
        std::map<int , Client *>::const_iterator it = clients.find(client_fd);
        if (it == clients.end())
            throw std::invalid_argument("Invalid client fd");
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error finding client: " << e.what() << '\n';
    }
    return (client_fd);
}

int Server::findConnectedClientByFd(int client_fd)
{
    try
    {
        std::map<int , Client *>::iterator it = clients.find(client_fd); 
        if (it == clients.end())
            throw std::invalid_argument("Invalid client fd");
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error finding client: " << e.what() << '\n';
    }
    return (client_fd);
}

int Server::checkIfNewClient(const char *buffer, int client_fd)
{
    Client *newClient;
    //! Make sure the client_fd exits
    if (clientsTryingToConnect.count(client_fd) == 0)
    {
        newClient = new Client;
        newClient->setFd(client_fd);
        clientsTryingToConnect[client_fd] = newClient;
    }
    std::string tmp(buffer);
    if (tmp.find("PASS ") != std::string::npos)
    {
        //----------------------------------------------------
        //! we use a map instead a vector to store new clients trying to connect
        clientsTryingToConnect[client_fd] = newClient;
        //---------------------------------------------------
        addPassword(client_fd, extractCommandContent(tmp, "PASS "));
    }
    if (tmp.find("NICK ") != std::string::npos)
        addNick(client_fd, extractCommandContent(tmp, "NICK "));
    if (tmp.find("USER ") != std::string::npos)
    {
        addUser(client_fd, extractCommandContent(tmp, "USER "));
        handleConnection(client_fd);
    }
    return (0);
}

void Server::addNick(int client_fd, const std::string &nick)
{
    try {
        //! Make sure the client_fd is valid
        if (clientsTryingToConnect.count(client_fd) == 0)
            throw std::invalid_argument("Invalid client_fd");

        //! Set the nickname of the client with the given client_fd
        clientsTryingToConnect[client_fd]->setNickname(nick);
    } catch (const std::exception &e) {
        std::cerr << "Error setting nickname: " << e.what() << std::endl;
    }
}

void Server::addUser(int client_fd, const std::string &user)
{
    try
    {
        //! Make sure the client_fd is valied
        if (clientsTryingToConnect.count(client_fd) == 0)
            throw std::invalid_argument("Invalid client_fd");
        
        //! Set the nickname of the client with the given client_fd
        clientsTryingToConnect[client_fd]->setUsername(user);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error setting username: " << e.what() << std::endl;
    }
    
    clientsTryingToConnect[client_fd]->setUsername(user);
}

void Server::addPassword(int client_fd, const std::string &pass)
{
    clientsTryingToConnect[client_fd]->setPassword(pass);
}


int Server::handleConnection(int client_fd)
{
    const std::string tmp(password);

    if (!tmp.compare(clientsTryingToConnect[client_fd]->getPassword()))
    {
        return (welcomeClient(client_fd));
    }
    else
    {
        return (wrongPassword(client_fd));
    }
    return (0);
}

int Server::welcomeClient(int client_fd)
{
    const std::string sPort(port);
    const std::string welcomeClient = ":localhost/" + sPort + " 001 " +
                                      clientsTryingToConnect[client_fd]->getNickname() + " :Welcome to the server\r\n";
    if (send(client_fd, welcomeClient.data(), welcomeClient.size(), 0) < 0)
    {
        std::cerr << "Send error\n";
        return (-1);
    }
    //! map intead of vector 
    clients[client_fd] = clientsTryingToConnect[client_fd];
    clientsTryingToConnect.erase(client_fd);
    return (0);
}

int Server::wrongPassword(int client_fd)
{
    const std::string wrongPW = ":Wrong password, try again!\r\n";
    if (send(client_fd, wrongPW.data(), wrongPW.size(), 0) < 0)
    {
        std::cerr << "Send error\n";
        return (-1);
    }
    clientsTryingToConnect.erase(client_fd);
    return (1);
}
