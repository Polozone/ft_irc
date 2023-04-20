#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <poll.h>
#include <netdb.h>
#include <signal.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>
#include <cstddef>
#include<cstdlib>
#include "../client/Client.hpp"
#include "../channel/Channel.hpp"
#include "../utils/string_utils.hpp"

#define SERVER_ADDR "127.0.0.1"

#define TRUE 1
#define FALSE 0

class Channel;
class Client;

int setPoll(int listener_fd);
int welcomeClient(int fd);

class Server
{

public:

    Server(const char *port, const char *password);
    ~Server();

    // SIG HANDLER
    friend void sigHandler(int sig);
    friend void freeResources(Server *server);

    // Server setup
    int launchServer();
    void        freeResources();


private:
    Server();
    Server(const Server &rhs);

    Server &operator=(const Server &rhs);

// Server setup
    int         getAddrinfo();
    int         getListenerSock();

    // Poll setup
    int         setPoll();
    int         polling();
    int         acceptIncomingConnection();
    int         readExistingConnection(int i);
    int         closeConnection(int i);
    int         closeConnectionByFd(int fd);
    int         findFdsIndex(int fdToFind);
    int         handleCtrlD(char *buffer);

    void        addToChannelList(Channel *toAdd);
    void        printChannelList();
    Channel*    findChannelByName(std::string channelName);
    void        printClientMaps() const;
    void        deleteAllChannel();
    void        deleteAllClients();
    void        deleteAllClientsTryingToConnect();

    // Commands
    // ************************************
    // |                                  |
    // |             COMMANDS             |
    // |                                  |
    // ************************************

    // Init commands
    void        setCommand(std::string &clientInput, int clientFd);
    void        callCommand(std::vector<std::string> inputClient, const std::string rawClientInput, int clientFd);
    
    // JOIN
    void        joinCommand(std::vector<std::string> command, int clientFd);
    
    // MODE

    void        parseChannelModeCommand(std::vector<std::string> command, int clientFd);
    void        executeFlags(std::vector<std::string> command, int clientFd, Channel *targetedChannel);
    void        modeOflag(char sign, Channel *targetedChannel, std::string modeClientTargeted, Client *caller);
    void        modeLflag(char sign, Channel *targetedChannel, std::string limitString);
    void        modeTflag(char sign, Channel *targetedChannel);
    void        modeVflag(char sign, Channel *targetedChannel, std::string clientName);
    void        modeMflag(char sign, Channel *targetedChannel);
    void        modePflag(char sign, Channel *targetedChannel);
    void        modeSflag(char sign, Channel *targetedChanneld);
    void        modeIflag(char sign, Channel *targetedChannel);

    // PONG - PONG
    int         pingCommand(int client_fd, const std::string &token) const;

    // NICK
    int         nickCommand(int client_fd, const std::string &nick);
    int         checkIfNickAvailable(const std::string &nick) const;
    int         checkNickUser(int client_fd, const std::string &nick);

    // PRIVMSG
    void        privmsgCommand(Client &client, std::vector<std::string> args);

    // KILL
    void        killCommand(int clientFd, const std::string &clientName);

    // OPER
    void        OperCommand(Client &client, const std::vector<std::string> &args);
    void        addOperCreds(std::string user, std::string password);
    bool        checkOperCreds(const std::string &username, const std::string &password) const;

    // USER MODE COMMAND
    void parseUserModeCommand(Client &client, const std::vector<std::string> &commandArgs);

    // PART
    int partCommand(int client_fd, std::vector<std::string> clientInput);
    int getRealNameFromUserName(int client_fd, std::string &realname);

    //NOTICE
    void noticeCommand(Client &client, const std::vector<std::string> &inputClient);

    // KICK
    int         kickCommand(int client_fd, std::vector<std::string> inputClient);

    // ************************************
    // |           END COMMANDS           |
    // ************************************

    // Add new Client
    int checkIfNewClient(const char *buffer, int client_fd);
    void    addUser(int client_fd, const std::string &user);
    void    addPassword(int client_fd, const std::string &pass);
    int     handleConnection(int client_fd);
    int     welcomeClient(int client_fd);
    int     wrongPassword(int client_fd);
    int     isValidFd(int client_fd) const;
    void    addClientToList(Client *toAdd);

    // Utils
    int     findClientByFd(int client_fd) const;
    Client  &getClientByFd(int client_fd) const;
    // Client  *getClientByNickname(std::string const & nickname);
    Client  *findClientByNick(const std::string &nickname);
    int     removeClientFromMap(int client_fd);

    // DEBUG FUNCTIONS
    void    printClientList() const;
    void handleModeCommand(const std::vector<std::string> &inputClient, int clientFd);

    // Variables
    const char *port;
    const char *password;
    std::string _serverName;
    int listen_sd;
    int end_server;
    int close_conn;
    std::vector<struct pollfd> fds;
    struct addrinfo *servinfo;
    int concatenate;

    // Containers
    std::string concatenatedCmd;
    std::map<int, Client *>::iterator _it;
    std::map<int, Client *> _clients;
    std::map<int, Client *> _clientsTryingToConnect;
    std::map<std::string, std::string> _operatorCredentials;
    std::vector<Channel*>               _channelList;

};

// Server Utils
int         handleServerErrors(const char *str, int *sd);
int         detectEOF(const char *str);
struct      pollfd createPollFdNode(int sd, int event);
const       std::string extractCommandContent(const std::string &buffer, const std::string &command);
void        sendNumericReplies(int fd, const std::string &message);
std::string extractAndConcatenateStrings(std::vector<std::string> strings, int index);
void        sigHandler(int sig);
void        freeResources(Server *server);
