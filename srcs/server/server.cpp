/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theodeville <theodeville@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 09:30:30 by theodeville       #+#    #+#             */
/*   Updated: 2023/03/10 15:29:52 by theodeville      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server.hpp"

int handleServerErrors(const char *str, int *sd) {
    perror(str);
    if (*sd) {
        close(*sd);
    }
    return (1);
}

int getAddrinfo(struct addrinfo *&servinfo) {
    int status;
    struct addrinfo hints;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(SERVER_ADDR, SERVER_PORT, &hints, &servinfo)) != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return (-1);
    }
    return (0);
}

int getListenerSock(const struct addrinfo *servinfo) {
    int listen_sd;
    int status;
    int on = 1;
    
    listen_sd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (listen_sd == -1) {
        return (handleServerErrors("socket()", NULL));
    }

    status = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR,
                  (char *)&on, sizeof(on));
    if (status < 0) {
        return (handleServerErrors("sockopt()", &listen_sd));
    }

    int flags = fcntl(listen_sd, F_GETFL, 0);
    status = fcntl(listen_sd, F_SETFL, flags | O_NONBLOCK);
    if (status < 0) {
        return (handleServerErrors("fcntl()", &listen_sd));
    }

    status = bind(listen_sd, servinfo->ai_addr, servinfo->ai_addrlen);
    if (status < 0) {
        return (handleServerErrors("bind()", &listen_sd));
    }

    status = listen(listen_sd, 30);
    if (status < 0) {
        return (handleServerErrors("listen()", &listen_sd));
    }

    return (listen_sd);
}

int launchServer() {
    int listen_sd;
    struct addrinfo *servinfo;

    getAddrinfo(servinfo);
    
    listen_sd = getListenerSock(servinfo);
    if (listen_sd < 0) {
        std::cerr << "Error getting the listener_sock\n";
        return (-1);
    }
    
    setPoll(listen_sd);
    
    freeaddrinfo(servinfo);
    return (0);
}
