/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 06:47:03 by alexanderva       #+#    #+#             */
/*   Updated: 2023/03/30 10:01:02 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/**
    @brief This function implements the IRC OPER command, which authenticates\
    a client as an operator and grants them operator privileges.
    @param client A reference to a Client object representing the client who\
    issued the OPER command.
    @param args A constant reference to a vector of strings representing the\
    arguments passed to the OPER command.
    @note This function sends error messages to the client if the OPER command\
     is missing required parameters or if the provided credentials do not match\
      the predefined operator credentials.
*/
void Server::OperCommand(Client &client, const std::vector<std::string> &args)
{
  const std::string &nickname = client.getNickname();

  if (args.size() < 2)
  {
    client.sendMessage(ERR_NEEDMOREPARAMS(nickname));
    return;
  }

  const std::string &username = args[1];
  const std::string &password = args[2];

  // Check if the provided username and password match the predefined operator\
   credentials.
  if (checkOperCreds(username, password))
  {
    // Grant operator privileges to the client
    std::string message = ":" + this->_serverName + " MODE " + nickname + " +o";
    client.setOperatorStatus(true);
    client.sendMessage(RPL_YOUREOPER(nickname));
    client.sendMessage(message);

  }
  else
  {
    client.setOperatorStatus(false);
    client.sendMessage(ERR_PASSWDMISMATCH(nickname));
  }
}