#pragma once

#include <iostream>
#include <cmath>
#include <unistd.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/socket.h>
#include "world.hh"

#define HTTP_METHOD_POST "POST"
#define HTTP_METHOD_GET "GET"

enum HTTP_CODE
{
  NOT_FOUND_ERROR = 404,
  SERVER_ERROR = 500
};

inline unsigned int number_length(int n);
std::string * split(std::string s, char d);//, size_t tab_size);
void listen_socket(struct sockaddr_un * socket_addr, int sockfd, World * world, bool * keep_alive);
