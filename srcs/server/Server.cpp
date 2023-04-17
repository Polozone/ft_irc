
#include "./Server.hpp"

// CLASS INIT
Server::Server(const char *port, const char *password)
    : port(port), password(password), end_server(0), close_conn(0), concatenate(0), concatenatedCmd("")
{
    Server::addOperCreds("Admin", "42lyon");
}

Server::~Server() { }
Server::Server() {}
Server::Server(const Server &rhs)
{
    *this = rhs;
}

Server  &Server::operator=(const Server &rhs)
{
    port = rhs.port;
    password = rhs.password;
    listen_sd = rhs.listen_sd;
    fds = rhs.fds;
    return (*this);
}


// MEMBER FUNCTIONS
int Server::getAddrinfo() {
    int status;
    struct addrinfo hints;
    char hostname[NI_MAXHOST];

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(SERVER_ADDR, port, &hints, &servinfo)) != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return (-1);
    }

     // Extract the hostname from the first addrinfo struct in the list
    status = getnameinfo(servinfo->ai_addr, servinfo->ai_addrlen, hostname, sizeof(hostname), NULL, 0, 0);
    if (status != 0)
    {
        // Print an error message if getnameinfo fails
        std::cerr << "getnameinfo: " << gai_strerror(status) << std::endl;
        return (-1);
    }
    this->_serverName = hostname;
    return (0);
}

//! find client by FD
int Server::findClientByFd(int client_fd) const
{
    try
    {
        std::map<int, Client *>::const_iterator _const_it = _clients.find(client_fd);
        if (_const_it == _clients.end())
            throw std::invalid_argument("Invalid client fd");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error finding client: " << e.what() << '\n';
    }
    return (client_fd);
}

//! find client by nickname
Client *Server::findClientByNick(const std::string &nickname)
{
    for (_it = _clients.begin(); _it != _clients.end(); ++_it)
    {
        if (_it->second->getNickname() == nickname)
            return _it->second;
    }
    return 0; //standard c++98
}

int Server::getListenerSock() {
    int status;
    int on = 1;
    
    listen_sd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (listen_sd == -1) {
        return (handleServerErrors("socket()", NULL));
    }

    status = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    if (status < 0) {
        return (handleServerErrors("sockopt()", &listen_sd));
    }

    status = fcntl(listen_sd, F_SETFL, O_NONBLOCK);
    if (status < 0) {
        return (handleServerErrors("fcntl()", &listen_sd));
    }

    status = bind(listen_sd, servinfo->ai_addr, servinfo->ai_addrlen);
    if (status < 0) {
        return (handleServerErrors("bind()", &listen_sd));
    }

    status = listen(listen_sd, 30);
    if (status < 0) {
        return (handleServerErrors("listen()", &listen_sd));
    }

    return (listen_sd);
}

/**
    @brief Launches the server and sets up necessary connections and polling.
    This function first calls getAddrinfo() to obtain address information about the server.
    It then initializes the socket, modifies it to not block, assigns an IP and port to the socket,
    and marks it as passive in order to accept incoming connections. After setting up the socket,
    it creates a new poll instance to watch events and either establish new connections or handle
    commands. Finally, it frees the address info and returns 0.
    @return 0 if the server is successfully launched.
*/
int Server::launchServer() {

    getAddrinfo();
    listen_sd = getListenerSock();
    setPoll();
    freeaddrinfo(servinfo);
    return (0);
}

void    Server::printChannelList()
{
    std::vector<Channel*>::iterator it;

    for (it = _channelList.begin(); it != _channelList.end(); ++it)
    {
        Channel* channel = *it;
        std::cout << "channelName:" << channel->getChannelName() <<std::endl;
    }
}

Channel*    Server::findChannelByName(std::string channelName)
{
    std::vector<Channel *>::iterator it;

    for (it = _channelList.begin(); it != _channelList.end(); ++it)
    {
        if ((*it)->getChannelName() == channelName)
            return (*it);
    }
    return (NULL);
}

void    Server::addClientToList(Client *toAdd)
{
    _clients.insert(std::pair<int, Client*>(toAdd->getFd(), toAdd));
}

void    Server::printClientList() const
{
    std::map<int, Client *>::const_iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        std::cout << it->first << std::endl;
        std::cout << it->second->getNickname() << std::endl;
    }
}

/**
    @brief Deletes all channels stored in the server's channel list.
    This function iterates through the server's channel list, deletes each channel object,
    and sets the corresponding vector element to NULL. After deletion, the function also
    clears the entire channel list.
*/
void Server::deleteAllChannel()
{
    std::vector<Channel *>::iterator it;
    std::vector<Channel *>::iterator ite = _channelList.end();

    for (it = _channelList.begin(); it != ite; ++it)
    {
        if (*it != NULL)
        {
            std::cout << "Deleting channel at address: " << *it << std::endl;
            delete (*it);
            *it = NULL;
        }
    }
    _channelList.clear();
}

void Server::deleteAllClients()
{
    std::map<int, Client *>::iterator it;
    std::map<int, Client *>::iterator ite = _clients.end();

    std::cout << "passing by deleteAllClients before for loop\n";
    for (it = _clients.begin(); it != ite; ++it)
    {
        if (it->second != NULL)
        {
            std::cout << "passing by delete : clients\n";
            delete (it->second);
            it->second = NULL;
        }
    }
    _clients.clear();
}

void Server::deleteAllClientsTryingToConnect()
{
    std::map<int, Client *>::iterator it;
    std::map<int, Client *>::iterator ite = _clientsTryingToConnect.end();

    std::cout << "passing by deleteAllClientsTryingToConnect before for loop\n";
    for (it = _clientsTryingToConnect.begin(); it != ite; ++it)
    {
        if (it->second != NULL)
        {
            std::cout << "passing by delete : ClientsTryingToConnect\n";
            delete (it->second);
            it->second = NULL;
        }
    }
    _clientsTryingToConnect.clear();
}
