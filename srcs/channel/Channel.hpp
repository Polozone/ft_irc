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

    private:
        Channel();

        std::map<int, Client*>      _clients;
        std::vector<std::string>    _operators;
        std::string                 _channelName;
        std::string                 _passwd;

    //      // Ajout d'un élément à la map
    // MaClasse* obj = new MaClasse();
    // map.insert(std::make_pair(1, obj));

    // // Accès à un élément de la map
    // MaClasse* obj2 = map[1];

    // // Suppression d'un élément de la map
    // map.erase(1);

};