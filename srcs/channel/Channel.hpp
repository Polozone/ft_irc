#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "../client/Client.hpp"

class Channel {

    public:

        Channel(std::string name, std::string passwd, Client * creator);
        Channel(std::string name, std::string passwd, Client * creator,
                bool isPrivate, bool _isSecret, bool _isInviteOnly,
                std::string _topic, int _maxClients);

        std::string                 getPasswd(){return _passwd;};
        std::string                 getChannelName(){return _channelName;};
        bool                        getPrivateStatus(){return _isPrivate;};
        bool                        getSecretStatus(){return _isSecret;};
        bool                        getInviteStatus(){return _isInviteOnly;};
        std::string                 getTopic(){return _topic;};
        int                         getMaxClient(){return _maxClients;};

        void                        setPasswd(std::string &passwd){_passwd = passwd;};
        void                        setChannelName(std::string &channelName){_channelName = channelName;};
        void                        setPrivateStatus(bool status){_isPrivate = status;};
        void                        setSecretStatus(bool status){_isSecret = status;};
        void                        setInviteStatus(bool status){_isInviteOnly = status;};
        void                        setTopic(std::string &topic){_topic = topic;};
        void                        setMaxClient(int maxClients){_maxClients = maxClients;};                        

        void                        addOperator(std::string &opName);
        void                        removeOperator(std::string &opName);
        void                        printOperatos();
        void                        addClientToChannel(int fdClient, Client *clientToAdd);
        void                        removeClientByFd(int fdClient);
        void                        addClientAllowed(std::string &nameAllowed);
        void                        removeClientAllowed(std::string &nameDisallowed);

    private:
        Channel();

        std::vector<std::string>::iterator  _it;
        std::map<int, Client*>              _clients;
        std::vector<std::string>            _operators;
        std::string                         _channelName;
        std::string                         _passwd;
        std::vector<std::string>            _privateClientAllowed;
        Client *                            _creator;

        int                                 _nbrClientsConnected;

        bool                                _isPrivate; // -p
        bool                                _isSecret;  // -s
        bool                                _isInviteOnly; // -i
        std::string                         _topic;         // -t
        int                                 _maxClients;    // -l

};