#pragma once

#include <iostream>
#include <map>
#include <cmath>
#include <unistd.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <nlohmann/json.hpp>
#include "world.hh"
#include "utils.hh"

#define HTTP_METHOD_POST "POST"
#define HTTP_METHOD_GET "GET"
#define REQUEST_READ_BUFFER 1024

enum HTTP_CODE
{
  OK,
  BAD_REQUEST,
  NOT_FOUND_ERROR,
  METHOD_NOT_ALLOWED,
  INTERNAL_SERVER_ERROR
};

const std::string HTTP_CODE_LITERALLY[] = {
  "200 OK",
  "400 Bad Request"
  "404 Not Found",
  "405 Method Not Allowed",
  "500 Internal Server"
};

inline unsigned int number_length(int n);
std::map<std::string, std::string> parse_change_world_request_data(std::string request);
void listen_socket(struct sockaddr_in * socket_addr, int sockfd, World * world, bool * keep_alive, bool * waiting_for_necessary_data);
