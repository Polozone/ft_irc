#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>
#include <netdb.h>
#include <iostream>
#include <poll.h>
#include <vector>

#define SERVER_PORT "15000"
#define SERVER_ADDR "127.0.0.1"

#define TRUE             1
#define FALSE            0

int launchServer();
int setPoll(int listener_fd);

