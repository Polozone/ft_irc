/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setPoll.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 13:10:58 by theodeville       #+#    #+#             */
/*   Updated: 2023/03/20 13:11:58 by alexanderva      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

int Server::closeConnection(int i)
{
    std::cout << "connection closed - " << fds[i].fd << std::endl;
    close(fds[i].fd);
    fds.erase(fds.begin() + i);
    close_conn = 0;
    return (0);
}

int Server::handleCtrlD(const char *buffer)
{
    std::string tmp(buffer);
    if (detectEOF(buffer))
    {
        concatenate = 1;
        concatenatedCmd += tmp;
        return (1);
    }
    else if (!detectEOF(buffer) && concatenate)
    {
        concatenate = 0;
        concatenatedCmd += tmp;
        std::cout << concatenatedCmd << std::endl;
        concatenatedCmd.empty();
        return (1);
    }
    return (0);
}

int Server::readExistingConnection(int i)
{
    int status;
    char buffer[1026] = {0};
    std::cout << "passing by here readExistingConnection\n";

    status = recv(fds[i].fd, buffer, sizeof(buffer), 0);
    if (status < 0)
    {
        if (errno != EWOULDBLOCK)
        {
            perror("  recv() failed");
            close_conn = TRUE;
        }
        return (-1);
    }
    if (status == 0)
    {
        close_conn = TRUE;
        return (0);
    }
    if (status > 0)
    {
        if (!handleCtrlD(buffer))
        {
            std::cout << buffer << "\n";
            memset(buffer, 0, sizeof(buffer));
        }
        return (0);
    }
    return (0);
}

int Server::acceptIncomingConnection()
{
    int new_sd;
    do
    {
        new_sd = accept(listen_sd, NULL, NULL);
        if (new_sd < 0)
        {
            if (errno != EWOULDBLOCK)
            {
                perror("  accept() failed");
                end_server = TRUE;
            }
            return (-1);
        }

        std::cout << "Connection accepted - " << new_sd << std::endl;
        std::cout << "passing by accepting connection\n";

        fds.push_back(createPollFdNode(new_sd, POLLIN | POLLHUP));
    } while (new_sd != -1);
    return (0);
}

int Server::polling()
{
    int status;

    std::cout << "Waiting on poll()...\n";
    status = poll(fds.data(), fds.size(), 180000);
    if (status < 0)
    {
        perror("poll()");
        return (-1);
    }

    if (status == 0)
    {
        std::cerr << "poll() timedout\n";
        return (-1);
    }
    return (0);
}

int Server::setPoll()
{
    int current_size;

    //!----create poll instance assigning a fd to monitor\
    //!----and what tipe of event we want to monitor
    //!---- we add it to a list of fds, representing the users
    fds.push_back(createPollFdNode(listen_sd, POLLIN));

    do
    {
        if (polling() == -1)
            break;
        current_size = fds.size();
        for (int i = 0; i < current_size; i++)
        {
            //! if no event 
            if (fds[i].revents == 0)
                continue;
            //! if the file descriptor has hang up
            if (fds[i].revents & POLLHUP)
            {
                closeConnection(i);
                continue;
            }
            //! at this point if fd event different than POLLIN, we sent error 
            if (fds[i].revents != POLLIN)
            {
                printf("  Error! revents = %d\n", fds[i].revents);
                end_server = TRUE;
                break;
            }
            if (fds[i].fd == listen_sd)
            {
                // std::cout << fds[i].fd << " | listen_sd: " << listen_sd << "\n";
                if (acceptIncomingConnection() == -1)
                    break;
            }
            else
            {
                // std::cout << fds[i].fd << " | listen_sd: " << listen_sd << "\n";
                if (readExistingConnection(i) == -1)
                    break;
            }
            
            if (close_conn)
                closeConnection(i);
        }

    } while (end_server == FALSE);

    return (0);
}
