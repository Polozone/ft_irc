
#include "Channel.hpp"

void    Channel::addExistingChannels(std::string &channelName)
{
    
    std::vector<std::string>::iterator it = std::find(_existingChannels.begin(), _existingChannels.end(), channelName);
    if (it == _existingChannels.end())
        _existingChannels.push_back(channelName);
    else 
        std::cout << "You are trying to add " << channelName << " to the list of existing channel. There is already a channel of this name" << std::endl;
}

void    Channel::printExistingChannels(){
     for (std::vector<std::string>::iterator it = _existingChannels.begin(); it != _existingChannels.end(); ++it) {
        std::cout << *it << std::endl;
    }
}

void    joinCommand(std::string &userInput)
{
    if (userInput.substr(0, 4) == "JOIN")
        dprintf(2, "JOIN CMD\n");
}

void handlerUserInput()
{
    std::string input;

    while (true) {
        std::getline(std::cin, input);
        joinCommand(input);
        if (input == "exit") {
            break; // Sortie de la boucle si la chaîne est "exit"
        }
    }
}