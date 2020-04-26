#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>

#include "util.h"

#define HTTP_METHOD_POST "POST"
#define HTTP_METHOD_GET "GET"
#define REQUEST_READ_BUFFER 1024

#define NOT_FOUND_ERROR "404 Not Found"

void listen_socket(struct sockaddr_in * socket_addr, int sockfd);

#endif
