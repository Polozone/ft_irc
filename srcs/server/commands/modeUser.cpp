/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeUser.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 07:38:50 by alexanderva       #+#    #+#             */
/*   Updated: 2023/03/30 09:44:23 by alexanderva      ###   ########.fr       */
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

std::string getModeString(int userMode)
{
    std::vector
    std::map<int, int, std::string> modes {
        { (0 << 0), "none"},
        { (1 << 0), "a"},
        { (1 << 1), "w"},
        { (1 << 2), "i"},
        { (1 << 3), "o"}, 
        { (1 << 4), "s"},
        { (1 << 5), "r"}

    };
}

void modeCommand(Client &client, const std::vector<std::string> &commandArgs)
{
    std::string const &nickname = client.getNickname();
    std::string result = "";

    if (commandArgs.size() < 2)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(nickname));
        return;
    }
    if (nickname != commandArgs[0])
    {
        client.sendMessage(ERR_USERSDONTMATCH(nickname));
        return;
    }
    const std::string &mode = commandArgs[1];

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
    }

    client.addMode(getModeFlag(&modeFlag));
    RPL_UMODEIS(nickname, modeFlag); //send numeric reply for success user mode change

    return ;
}