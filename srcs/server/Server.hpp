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
#include <algorithm>

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

    int getAddrinfo();
    int launchServer();
    int getListenerSock();

    int setPoll();
    int polling();
    int acceptIncomingConnection();
    int readExistingConnection(int i);
    int closeConnection(int i);
    int handleCtrlD(const char *buffer);

    void    addExistingChannels(const std::string &channelName);
    void    printExistingChannels();
    
    void    parseCommand(std::string &userInput);

    const char                  *port;
    const char                  *password;
    int                         listen_sd;
    int                         end_server;
    int                         close_conn;
    std::vector<struct pollfd>  fds;
    struct addrinfo             *servinfo;
    int                         concatenate;
    std::string                 concatenatedCmd;

};
