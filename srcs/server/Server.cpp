
#include "./Server.hpp"

// CLASS INIT
Server::Server(const char *port, const char *password)
    : port(port), password(password), end_server(0), close_conn(0), concatenate(0), concatenatedCmd("")
{
    Client *name = new Client(4, "hostnamectl");
    Client *name2 = new Client(5, "hostnamectl2");
    Client *name3 = new Client(6, "hostnamectl3");
    name->setNickname("Paul");
    name2->setNickname("Jean");
    name3->setNickname("Jacques");
    addClientToList(name);
    addClientToList(name2);
    addClientToList(name3);
    // printClientList();
    // Server::launchServer();
    std::string input;
        std::getline(std::cin, input);
        setCommand(input, 4);
        std::getline(std::cin, input);
        setCommand(input, 5);
        std::getline(std::cin, input);
        setCommand(input, 6);
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

    listen_sd = getListenerSock();
    
    setPoll();
    
    freeaddrinfo(servinfo);
    return (0);
}

void    Server::addToChannelList(Channel *toAdd)
{
    std::vector<Channel*>::iterator it;
    for (it = _channelList.begin(); it != _channelList.end(); ++it)
    {
        Channel* tmp = *it;
        if (tmp->getChannelName() == toAdd->getChannelName())
            break ;
    }
    if (it == _channelList.end())
        _channelList.push_back(toAdd);
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
    clients.push_back(toAdd);
}

void    Server::printClientList()
{
    std::vector<Client *>::iterator it;

    for (it = clients.begin(); it != clients.end(); ++it)
        std::cout << (*it)->getNickname() << std::endl;
}
