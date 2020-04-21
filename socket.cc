#include "socket.hh"

inline unsigned int number_length(int n)
{
  return trunc(log10(n)) + 1;
}

std::map<std::string, std::string> parse_change_world_request_data(std::string request)
{
  std::map<std::string, std::string> request_data;

  size_t tab_size = 0;
  std::string * parsed_request = split(request, '\n', &tab_size);
  std::string * parsed_request_data = split(parsed_request[tab_size - 1], ',', &tab_size);

  for (int i = 0; i < tab_size; i++)
  {
    std::string * values = split(parsed_request_data[i], ':');
    request_data[values[0]] = values[1];
  }

  delete[] parsed_request;
  delete[] parsed_request_data;
  return request_data;
}

void listen_socket(struct sockaddr_in * socket_addr, int sockfd, World * world, bool * keep_alive, bool * waiting_for_necessary_data)
{
  int highest_sockfd = sockfd;
  fd_set sockets_read, sockets_write, sockets_error, sockets_copy;
  FD_ZERO(&sockets_read);
  FD_ZERO(&sockets_write);
  FD_ZERO(&sockets_error);
  FD_ZERO(&sockets_copy);
  FD_SET(sockfd, &sockets_copy);

  while (*keep_alive)
  {
    sockets_read = sockets_copy;
    sockets_write = sockets_copy;
    sockets_error = sockets_copy;

    int socket_activity = select(highest_sockfd + 1, &sockets_read, &sockets_write, &sockets_error, NULL);

    if (socket_activity < 0)
    {
      std::cout << "Error: Select." << std::endl;
    }

    if (FD_ISSET(sockfd, &sockets_read))
    {
      unsigned int socket_len = 0;
      struct sockaddr_in new_socket;

      //Handle new connection
      int new_sockfd = accept(sockfd, (struct sockaddr*)&new_socket, &socket_len);

      if (new_sockfd < 0)
      {
        std::cout << "Error: Accept new connection." << std::endl;
      }

      if (new_sockfd > highest_sockfd) highest_sockfd = new_sockfd;

      FD_SET(new_sockfd, &sockets_copy);
    }

    for (int i = sockfd + 1; i <= highest_sockfd; i++)
    {
      //if (FD_ISSET(i, &sockets_error))

      if (FD_ISSET(i, &sockets_error))
      {
        if (i == highest_sockfd) highest_sockfd--;

        close(i);
        FD_CLR(i, &sockets_copy);
        continue;
      }

      if (FD_ISSET(i, &sockets_read) && FD_ISSET(i, &sockets_write))
      {
        //Handle request
        char read_buffer[REQUEST_READ_BUFFER];
        ssize_t socket_bytes_read = read(i, &read_buffer, REQUEST_READ_BUFFER);

        if (socket_bytes_read <= 0)
        {
          close(i);
          FD_CLR(i, &sockets_copy);
          continue;
        }

        read_buffer[socket_bytes_read] = '\0';
        std::string * request_lines = split(read_buffer, '\n');
        std::string * request_details = split(read_buffer, ' ');
        std::string send_buffer = "HTTP/1.0 ";
        std::string * path = split(request_details[1], '/');

        if (path[1] == "world")
        {
          if (request_details[0] == HTTP_METHOD_GET || request_details[0] == "")
          {
            if (path[2] == "population") send_buffer += "200 OK\nContent-Type: text/plain\nContent-Length: " + std::to_string(number_length(world->get_population())) + "\n\n" + std::to_string(world->get_population());
            else if (path[2] == "susceptible") send_buffer += "200 OK\nContent-Type: text/plain\nContent-Length: " + std::to_string(number_length(world->get_susceptible())) + "\n\n" + std::to_string(world->get_susceptible());
            else if (path[2] == "infected") send_buffer += "200 OK\nContent-Type: text/plain\nContent-Length: " + std::to_string(number_length(world->get_infected())) + "\n\n" + std::to_string(world->get_infected());
            else if (path[2] == "recovered") send_buffer += "200 OK\nContent-Type: text/plain\nContent-Length: " + std::to_string(number_length(world->get_recovered())) + "\n\n" + std::to_string(world->get_recovered());
            else send_buffer += HTTP_CODE_LITERALLY[HTTP_CODE::NOT_FOUND_ERROR];
          }
          else if (request_details[0] == HTTP_METHOD_POST)
          {

          }
          else
          {
            send_buffer += HTTP_CODE_LITERALLY[HTTP_CODE::METHOD_NOT_ALLOWED];
          }
        }
        else if (path[1] == "system")
        {
          if (request_details[0] == HTTP_METHOD_GET)
          {
            if (path[2] == "status")
            {
              std::string status = (*waiting_for_necessary_data ? "waiting" : "calculating");
              send_buffer += "200 OK\nContent-Type: text/plain\nContent-Length: " + std::to_string(status.length()) + "\n\n" + status;
            }
          }
          else if (request_details[0] == HTTP_METHOD_POST)
          {
            if (path[2] == "off")
            {
              //Turn off the server
              *keep_alive = false;
              //world->set_keep_alive(false);
            }
            else if (path[2] == "reset")
            {
              std::map<std::string, std::string> request_data = parse_change_world_request_data(read_buffer);

              if (request_data.find("population") != request_data.end()) world->set_population(std::stoul(request_data["population"]));
              else if (request_data.find("probability_of_cure") != request_data.end()) world->set_probability_of_cure(std::stof(request_data["probability_of_cure"]));
              else if (request_data.find("probability_of_infection") != request_data.end()) world->set_probability_of_infection(std::stof(request_data["probability_of_infection"])/world->get_population());
              else if (request_data.find("susceptible") != request_data.end()) world->set_susceptible(std::stoul(request_data["susceptible"]));
              else if (request_data.find("infected") != request_data.end()) world->set_infected(std::stoul(request_data["infected"]));
              else if (request_data.find("recovered") != request_data.end()) world->set_recovered(std::stoul(request_data["recovered"]));

              *waiting_for_necessary_data = false;
              send_buffer += HTTP_CODE_LITERALLY[HTTP_CODE::OK];
            }
            else
            {
              send_buffer += HTTP_CODE_LITERALLY[HTTP_CODE::NOT_FOUND_ERROR];
            }
          }
          else
          {
            send_buffer += HTTP_CODE_LITERALLY[HTTP_CODE::METHOD_NOT_ALLOWED];
          }
        }
        else
        {
          send_buffer += HTTP_CODE_LITERALLY[HTTP_CODE::NOT_FOUND_ERROR];
        }

        delete[] request_lines;
        delete[] request_details;
        delete[] path;

        ssize_t sockets_bytes_send = send(i, send_buffer.c_str(), send_buffer.length(), 0);

        if (i == highest_sockfd) highest_sockfd--;

        close(i);
        FD_CLR(i, &sockets_copy);
      }
    }

  }

  for (int i = sockfd + 1; i <= highest_sockfd; i++)
  {
    close(i);
  }
}
