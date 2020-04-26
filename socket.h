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

#define HTTP_OK "HTTP/1.0 200 OK\nContent-Type: text/html\n\n"
#define HTTP_NOT_FOUND_ERROR "HTTP/1.0 404 Not Found\n"
#define HTTP_INTERNAL_SERVER_ERROR "HTTP/1.0 500 Internal Server\n"

void listen_socket(struct sockaddr_in * socket_addr, int sockfd);

#endif
