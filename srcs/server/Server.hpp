#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>
#include <netdb.h>
#include <iostream>
#include <poll.h>
#include <vector>
#include <map>

#include "../client/Client.hpp"

#define SERVER_ADDR "127.0.0.1"

#define TRUE             1
#define FALSE            0

int setPoll(int listener_fd);
int welcomeClient(int fd);


class Server {

public:
    Server(const char *port, const char *password);
    ~Server();

private:
    Server();
    Server(const Server &rhs);

    Server  &operator=(const Server &rhs);

    // Server setup
    int launchServer();
    int getAddrinfo();
    int getListenerSock();

    // Poll setup
    int setPoll();
    int polling();
    int acceptIncomingConnection();
    int readExistingConnection(int i);
    int closeConnection(int i);
    int handleCtrlD(const char *buffer);

    // Add new Client
    int addNewClient();


    const char                      *port;
    const char                      *password;
    int                             listen_sd;
    int                             end_server;
    int                             close_conn;
    std::vector<struct pollfd>      fds;
    struct addrinfo                 *servinfo;
    int                             concatenate;
    std::string                     concatenatedCmd;
    std::map<const int, Client *>   _clientList;
    std::map<int, std::string>      _buffersByFd;
};

// Server Utils
int handleServerErrors(const char *str, int *sd);
int detectEOF(const char *str);
struct pollfd createPollFdNode(int sd, int event);
