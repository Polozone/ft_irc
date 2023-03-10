/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theodeville <theodeville@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 09:30:30 by theodeville       #+#    #+#             */
/*   Updated: 2023/03/10 10:10:27 by theodeville      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server.hpp"

int handleError(const char *str, int *sd) {
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
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(SERVER_ADDR, SERVER_PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return (-1);
    }
    return (0);
}

int launchServer() {
    int listen_sd, new_sd;
    int on = 1;
    int status;
    struct addrinfo *servinfo;

    getAddrinfo(servinfo);
    
    listen_sd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (listen_sd == -1) {
        return (handleError("socket()", NULL));
    }

    status = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR,
                  (char *)&on, sizeof(on));
    if (status < 0) {
        return (handleError("sockopt()", &listen_sd));
    }

    int flags = fcntl(listen_sd, F_GETFL, 0);
    status = fcntl(listen_sd, F_SETFL, flags | O_NONBLOCK);
    if (status < 0) {
        return (handleError("fcntl()", &listen_sd));
    }

    status = bind(listen_sd, servinfo->ai_addr, servinfo->ai_addrlen);
    if (status < 0) {
        return (handleError("bind()", &listen_sd));
    }

    status = listen(listen_sd, 30);
    if (status < 0) {
        return (handleError("listen()", &listen_sd));
    }

    new_sd = accept(listen_sd, NULL, NULL);
    char buffer[100];
    recv(new_sd, buffer, sizeof(buffer), 0);

    return (0);
}
