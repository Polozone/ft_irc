#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cstddef>
#include "../client/Client.hpp"
#include "../server/numericReplies.hpp"

class Server;

class Channel {

    public:

        Channel(std::string name, std::string passwd, Client * creator);

        // GETTERS
        std::string                 getPasswd(){return _passwd;};
        std::string                 getChannelName(){return _channelName;};
        bool                        getPrivateStatus(){return _isPrivate;};
        bool                        getSecretStatus(){return _isSecret;};
        bool                        getInviteStatus(){return _isInviteOnly;};
        bool                        getTopicStatus(){return _topic;};
        int                         getMaxClient(){return _maxClients;};

        // SETTERS
        void                        setPasswd(std::string &passwd){_passwd = passwd;};
        void                        setChannelName(std::string &channelName){_channelName = channelName;};
        void                        setPrivateStatus(bool status){_isPrivate = status;};
        void                        setSecretStatus(bool status){_isSecret = status;};
        void                        setInviteStatus(bool status){_isInviteOnly = status;};
        void                        setTopicStatus(bool status){_topic = status;};
        void                        setMaxClient(int maxClients){_maxClients = maxClients;};                        
        void                        setStatusModerate(bool status){_isModerate = status;};                        
        void                        setTopicContent(std::string content){_topicContent = content;};

        void                        addOperator(std::string opName);
        void                        removeOperator(std::string &opName);
        void                        addClientToChannel(int fdClient, Client *clientToAdd);
        void                        removeClientByFd(int fdClient);
        void                        addClientAllowed(std::string &nameAllowed);
        void                        removeClientAllowed(std::string &nameDisallowed);
        void                        addInvitedClient(std::string toAdd);

        void                        addClientToSpeakList(std::string &clientName);
        void                        rmvClientFromSpeakList(std::string &clientName);
        void                        sendMsgToSpeakList(std::string &message);

        bool                        isClientExist(std::string &clientName);
        bool                        isClientBan(std::string& clientName);
        bool                        isClientIsInvited(std::string &clientName);
        bool                        isOperator(std::string clientName);

        void                        sendToChannel(const std::string &message, const Client &user);
        void                        sendToAllClients(const std::string &message);

        Client *                    findClientByFd(int fd);

        //tools
        void                        printClientList();
        void                        printOperators();
        void                        printSpeakList();
        Client                      *findClient(int client_fd);

    private:
        Channel();

        std::vector<std::string>::iterator  _it;
        std::map<int, Client*>::iterator    _itm;
        std::map<int, Client*>              _clients;
        std::string                         _channelName;
        std::string                         _passwd;
        Client *                            _creator;

        std::vector<std::string>            _operators;
        std::vector<std::string>            _privateClientAllowed;
        std::vector<std::string>            _invitedClient;
        std::vector<std::string>            _canSpeakList;
        
        int                                 _nbrClientsConnected;
        bool                                _isPrivate; // -p  -> dont list channel form outside with /list
        bool                                _isSecret;  // -s -> idem
        bool                                _isInviteOnly; // -i -> user accepted if they were invited by operators
        bool                                _topic;         // -t -> set a topic of channel
        int                                 _maxClients;    // -l -> limits of clients
        bool                                _isModerate;   // -m --> only operators or -v users
        bool                                _isVoice;     // -v -> grants user to speak on -m channel
        std::string                         _topicContent; // content of the topic
};
