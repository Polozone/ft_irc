/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setPoll.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theodeville <theodeville@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 13:10:58 by theodeville       #+#    #+#             */
/*   Updated: 2023/03/11 12:32:16 by theodeville      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server.hpp"

struct pollfd createPollFdNode(int sd, int event)
{
    struct pollfd pollFdNode = {
        .fd = sd,
        .events = event};

    return (pollFdNode);
}

int closeConnection(std::vector<struct pollfd> &fds, int i)
{
    std::cout << "connection closed - " << fds[i].fd << std::endl;
    close(fds[i].fd);
    fds.erase(fds.begin() + i);
    return (0);
}

int readExistingConnection(const std::vector<struct pollfd> &fds, int i, int &close_conn)
{
    int status;
    char buffer[1026];

    do
    {
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
            printf("  Connection closed\n");
            close_conn = TRUE;
            break;
        }
        if (status > 0)
        {
            std::cout << buffer << "\n";
            memset(buffer, 0, sizeof(buffer));
            break;
        }

    } while (TRUE);
    return (0);
}

int acceptIncomingConnection(int listener_fd, int &end_server,
                             std::vector<struct pollfd> &fds)
{
    int new_sd;
    do
    {
        new_sd = accept(listener_fd, NULL, NULL);
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

        fds.push_back(createPollFdNode(new_sd, POLLIN | POLLHUP));
    } while (new_sd != -1);
    return (0);
}

int polling(std::vector<struct pollfd> &fds)
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

int setPoll(int listener_fd)
{
    std::vector<struct pollfd> fds;
    int current_size;
    int end_server = FALSE;
    int close_conn;

    fds.push_back(createPollFdNode(listener_fd, POLLIN));

    do
    {
        if (polling(fds) == -1)
            break;
        current_size = fds.size();
        for (int i = 0; i < current_size; i++)
        {
            if (fds[i].revents == 0)
                continue;
            if (fds[i].revents & POLLHUP)
            {
                closeConnection(fds, i);
                continue;
            }
            if (fds[i].revents != POLLIN)
            {
                printf("  Error! revents = %d\n", fds[i].revents);
                end_server = TRUE;
                break;
            }

            if (fds[i].fd == listener_fd)
            {
                if (acceptIncomingConnection(listener_fd, end_server, fds) == -1)
                    break;
            }
            else
            {
                if (readExistingConnection(fds, i, close_conn) == -1)
                    break;
            }
            
            if (close_conn)
                closeConnection(fds, i);
        }

    } while (end_server == FALSE);

    return (0);
}
