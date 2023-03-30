#include "./Server.hpp"

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

int Server::checkIfNewClient(const char *buffer, int client_fd)
{
    if (isValidFd(client_fd) == -1)
        return (-1);
    if (_clients.count(client_fd))
        return (1);
    std::string tmp(buffer);
    if (tmp.find("PASS ") != std::string::npos)
    {
        // NO NEED TO DO THIS ANY MORE AS IT IS ALREADY
        // IMPLEMENTED IN FUNCTION acceptIncomingConnection()
        // IN FILE setPoll.cpp
        // //----------------------------------------------------
        // //! we use a map instead a vector to store new clients trying to connect
        // _clientsTryingToConnect[client_fd] = newClient;
        // //---------------------------------------------------
        addPassword(client_fd, extractCommandContent(tmp, "PASS "));
    }
    if (tmp.find("NICK ") != std::string::npos)
    {
        nickCommand(client_fd, extractCommandContent(buffer, "NICK "));
    }
    if (tmp.find("USER ") != std::string::npos)
    {
        addUser(client_fd, extractCommandContent(tmp, "USER "));
        handleConnection(client_fd);
    }
    return (0);
}

void Server::addUser(int client_fd, const std::string &user)
{
    try
    {
        //! Make sure the client_fd is valied
        if (_clientsTryingToConnect.count(client_fd) == 0)
            throw std::invalid_argument("Invalid client_fd");
        
        //! Set the nickname of the client with the given client_fd
        _clientsTryingToConnect[client_fd]->setUsername(user);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error setting username: " << e.what() << std::endl;
        return ;
    }
    
    _clientsTryingToConnect[client_fd]->setUsername(user);
}

void Server::addPassword(int client_fd, const std::string &pass)
{
    _clientsTryingToConnect[client_fd]->setPassword(pass);
}


int Server::handleConnection(int client_fd)
{
    const std::string tmp(password);

    if (!tmp.compare(_clientsTryingToConnect[client_fd]->getPassword()))
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
                                      _clientsTryingToConnect[client_fd]->getNickname() + " :Welcome to the server\r\n";
    if (send(client_fd, welcomeClient.data(), welcomeClient.size(), 0) < 0)
    {
        std::cerr << "Send error\n";
        return (-1);
    }
    //! map intead of vector 
    _clients[client_fd] = _clientsTryingToConnect[client_fd];
    _clientsTryingToConnect.erase(client_fd);
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
    return (1);
}
