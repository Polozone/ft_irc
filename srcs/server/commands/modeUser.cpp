
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeUser.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 07:38:50 by alexanderva       #+#    #+#             */
/*   Updated: 2023/03/30 11:13:35 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
/**
    @brief Converts a flag string to the corresponding enum mode value.
    This function takes a flag string as input and searches for the corresponding
    enum mode value in a lookup table. If a match is found, the corresponding enum
    mode value is returned. If the input string is invalid, a default value is
    returned.
    @param flagString The flag string to convert to an enum mode value.
    @return The corresponding enum mode value for the input string, or a default
    value if the input is invalid.
    @note The lookup table is defined internally in the function and cannot be
    modified from outside.
    @note The input flag string must be one of the valid values defined in the
    lookup table.
*/
enum modes getModeFlag(const char *flagString)
{
    static const char *const flags[] = {"a", "w", "i", "o", "B", "s", "r"};
    for (int i = 0; i < std::string(flags).size(); i++)
    {
        if (strcmp(flagString, flags[i]) == 0)
        {
            return (enum modes)i;
        }
    }
    return none; // Return a default value in case of invalid input
}

/**
    @brief Gets the corresponding mode string for a given user mode integer.
    This function takes a user mode integer as input and searches for the corresponding
    mode string in a map. If a match is found, the corresponding mode string
    is returned. If the input integer is not found in the map, an exception is thrown.
    @param userMode The user mode integer to get the corresponding mode string for.
    @return The corresponding mode string for the input integer.
    @throw std::out_of_range if the input integer is not found in the map.
*/
std::string getModeString(int userMode)
{
    std::map<int, std::string> modes = {
        {(0 << 0), "none"},
        {(1 << 0), "a"},
        {(1 << 1), "w"},
        {(1 << 2), "i"},
        {(1 << 3), "o"},
        {(1 << 4), "s"},
        {(1 << 5), "r"}};
    try
    {
        // Attempt to find the user mode integer in the map
        std::map<int, std::string>::iterator it = modes.find(userMode);

        if (it == modes.end())
        {
            // If the integer is not found, throw an exception
            throw std::out_of_range("userMode not found in modes");
        }
    }
    catch (const std::exception &e)
    {
        // Catch the exception and print an error message to the standard error stream
        std::cerr << "Error: " << e.what() << '\n';
    }
}

/**
    @brief Handles the userMode command from a client.
    This function takes a Client object and a vector of command arguments as input.
    It checks if the number of arguments and the sender nickname are valid, and if the
    user mode flag is valid. If all checks pass, it adds the corresponding mode flag to
    the client's mode flags and sends a message to the client and other relevant parties.
    @param client The Client object representing the sender of the command.
    @param commandArgs The vector of command arguments, with the user mode flag as the second argument.
*/
void Server::userModeCommand(Client &client, const std::vector<std::string> &commandArgs)
{
    std::string const &nickname = client.getNickname();
    // Check if the number of arguments is valid
    if (commandArgs.size() < 2)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(nickname));
        return;
    }
    // Check if the sender nickname is valid
    if (nickname != commandArgs[0])
    {
        client.sendMessage(ERR_USERSDONTMATCH(nickname));
        return;
    }
    const std::string &mode = commandArgs[1];
    // Check if the mode string is valid
    if (mode.length() < 2 || mode[0] != '+' && mode[0] != '-')
    {
        client.sendMessage(ERR_UMODEUNKNOWNFLAG(nickname));
        return;
    }
    // Assuming that there is only one mode flag in the input for simplicity
    char modeFlag = mode[1];
    // Check if the mode flag is valid
    if (std::string("ai").find(modeFlag) == std::string::npos)
    {
        client.sendMessage(ERR_UMODEUNKNOWNFLAG(nickname));
        return;
    // Get the corresponding enum mode value for the mode flag
    enum modes modeValue = getModeFlag(&modeFlag);
    // Add the mode flag to the client's mode flags
    client.addMode(modeValue);
    // Construct the message to send to the client and other relevant parties
    std::string message = ":" + this->_serverName + " MODE " + nickname + " " + getModeString(modeValue);
    // Send a numeric reply to the client indicating success
    RPL_UMODEIS(nickname, modeFlag);
    
    return;
}
