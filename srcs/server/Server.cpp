#include "./Server.hpp"

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

int handleServerErrors(const char *str, int *sd) {
    perror(str);
    if (*sd) {
        close(*sd);
    }
    return (1);
}

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
    struct addrinfo *servinfo;

    getAddrinfo();

    listen_sd = getListenerSock();
    
    setPoll();
    
    freeaddrinfo(servinfo);
    return (0);
}

