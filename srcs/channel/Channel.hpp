#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "../client/Client.hpp"

class Channel {

    public:

        Channel(std::string name, std::string passwd);
        std::string                 getPasswd(){return _passwd;};
        std::string                 getChannelName(){return _channelName;};

        void                        addOperator(std::string &opName);
        void                        removeOperator(std::string &opName);
        void                        printOperatos();

    private:
        Channel();

        std::vector<std::string>::iterator  _it;
        std::map<int, Client*>              _clients;
        std::vector<std::string>            _operators;
        std::string                         _channelName;
        std::string                         _passwd;

        bool                                _isRestricted;
        int                                 _maxClients;
        int                                 _nbrClientsConnected;

};