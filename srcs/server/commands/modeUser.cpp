/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeUser.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 07:38:50 by alexanderva       #+#    #+#             */
/*   Updated: 2023/04/05 09:08:01 by alexanderva      ###   ########.fr       */
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
modes getModeFlag(const std::string &flagString)
{
    static const char *const flags[] = {"a", "w", "i", "s", "r"};
    static const modes values[] = {a, w, i, s, r};

    for (int i = 0; i < 5; i++)
    {
        if (flagString.compare(flags[i]) == 0)
        {
            return values[i];
        }
    }
    return none; // Return a default value in case of invalid input
}

/**
    @brief Converts an enum mode value to its corresponding string representation.
    This function takes an enum mode value and returns its corresponding string representation.
    The function searches for the matching mode value in an internal lookup table, and if a match is found,
    @param userMode The enum mode value to convert to its corresponding string representation.
    @return The string representation of the input enum mode value.
    @note The lookup table is defined internally in the function and cannot be modified from outside.
    @note The input enum mode value must be one of the valid values defined in the lookup table.
*/
std::string getModeString(modes userMode)
{
    static const modes mode_values[] = {a, w, i, s, r};
    static const char *const mode_strings[] = {"a", "w", "i", "s", "r"};
    const int num_modes = sizeof(mode_values) / sizeof(*mode_values);

    for (int i = 0; i < num_modes; i++)
    {
        if (mode_values[i] == userMode)
        {
            return mode_strings[i];
        }
    }
    return " ";
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
void Server::parseUserModeCommand(Client &client, const std::vector<std::string> &commandArgs)
{
    char modeFlag[2];
    std::string const &nickname = client.getNickname();
    // Check if the number of arguments is valid
    if (commandArgs.size() < 3)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(nickname));
        return;
    }
    // Check if the sender nickname is valid
    if (nickname != commandArgs[1])
    {
        client.sendMessage(ERR_USERSDONTMATCH(nickname));
        return;
    }
    const std::string &mode = commandArgs[2];
    // Check if the mode string is valid
    if (mode.length() < 2 || (mode[0] != '+' && mode[0] != '-'))
    {
        client.sendMessage(ERR_UMODEUNKNOWNFLAG(nickname));
        return;
    }
    // Assuming that there is only one mode flag in the input for simplicity
    modeFlag[0] = mode[1];
    modeFlag[1] = '\0';
    // Get the corresponding enum mode value for the mode flag
    if (std::string("aiwrs").find(modeFlag) == std::string::npos)
    {
        client.sendMessage(ERR_UMODEUNKNOWNFLAG(nickname));
        return;
    }
    enum modes modeValue = getModeFlag(modeFlag);
    // Check if the mode flag is valid
    // Add the mode flag to the client's mode flags
    client.addMode(modeValue);
    // Construct the message to send to the client and other relevant parties
    std::string message = ":" + this->_serverName + " MODE " + nickname + " " + mode[0] \
    + getModeString(modeValue);
    // Send a numeric reply to the client indicating success
    client.sendMessage(message);
    // client.sendMessage(RPL_UMODEIS(nickname, modeFlag));
    return;
}