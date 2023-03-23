
#include "./Server.hpp"

// CLASS INIT
Server::Server(const char *port, const char *password)
    : port(port), password(password), end_server(0), close_conn(0), concatenate(0), concatenatedCmd("")
{
    Server::launchServer();
}

Server::~Server() {}
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

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(SERVER_ADDR, port, &hints, &servinfo)) != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return (-1);
    }
    return (0);
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

int Server::launchServer() {

    getAddrinfo();

    //?----(socket)initialize socket
    //?----(fcntl)modify the socket to not block, even if there no data to read
    //?----bind(assign) a ip and port to the socker
    //?----listen, marck socket as passive in order to accept incoming connections 
    listen_sd = getListenerSock();
    
    //?---create new poll instance to watch event
    //? ---watch events to either stablish new connections or handle commands
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

Channel*    Server::findChannelByName(std::string channelName, int fdClient)
{
    std::vector<Channel *>::iterator it;

    for (it = _channelList.begin(); it != _channelList.end(); ++it)
    {
        if ((*it)->getChannelName() == channelName)
            return (*it);
    }
    // std::map<int, Client*>::iterator _it = clients.find(2);
    // if (_it != clients.end()) {
    //     Client* tmp = _it->second;
    //     sendNumericReplies(fdClient, ERR_NOSUCHCHANNEL(tmp->getNickname()));
    // }
    return (NULL);
}

void    Server::addClientToList(Client *toAdd)
{
    clients.insert(std::pair<int, Client*>(toAdd->getFd(), toAdd));
}

void    Server::printClientList()
{
    std::map<int, Client *>::iterator it;

    for (it = clients.begin(); it != clients.end(); ++it)
    {
        std::cout << it->first << std::endl;
        std::cout << it->second->getNickname() << std::endl;
    }
}

