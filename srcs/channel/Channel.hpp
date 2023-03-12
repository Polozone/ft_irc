#pragma once

#include "../client/Client.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

class Channel {

    public:

        void    addExistingChannels(std::string &channelName);
        void    printExistingChannels();

    private:

        std::vector<std::string>    _existingChannels;
        std::map<int, Client*>      _clients;

    //      // Ajout d'un élément à la map
    // MaClasse* obj = new MaClasse();
    // map.insert(std::make_pair(1, obj));

    // // Accès à un élément de la map
    // MaClasse* obj2 = map[1];

    // // Suppression d'un élément de la map
    // map.erase(1);

};