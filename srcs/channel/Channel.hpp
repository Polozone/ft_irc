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
        void                        addOperator();
        void                        removeOperator();

    private:
        Channel();

        std::map<int, Client*>      _clients;
        std::vector<std::string>    _operators;
        std::string                 _channelName;
        std::string                 _passwd;

};