#include "socket.hh"

bool is_file_exist(const char * path)
{
  FILE * file = fopen(path, "r");

  if (file)
  {
    fclose(file);
    return file;
  }
  return file;
}

inline unsigned int number_length(int n)
{
  return trunc(log10(n)) + 1;
}

std::string * split(std::string s, char d)//, size_t tab_size)
{
  size_t tab_size = 0;

  for (int i = 0; i < s.length(); i++)
  {
    if (s[i] == d) tab_size++;
  }

  std::string * result = new std::string[tab_size + 1];
  int index = 0;

  for (int i = 0; i < s.length(); i++)
  {
    if (s[i] == d)
    {
      index++;
    }
    else
    {
      result[index] += s[i];
    }
  }
  return result;
}

void listen_socket(struct sockaddr_un * socket_addr, int sockfd, World * world, bool * keep_alive)
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
      struct sockaddr_un new_socket;

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
        continue;
      }

      if (FD_ISSET(i, &sockets_read) && FD_ISSET(i, &sockets_write))
      {
        //Handle request
        char read_buffer[1000];
        ssize_t sockets_bytes_read = read(i, &read_buffer, 1000);

        if (sockets_bytes_read <= 0)
        {
          close(i);
          FD_CLR(i, &sockets_copy);
          continue;
        }

        read_buffer[sockets_bytes_read] = '\0';

        /*std::cout << "Read buffer: " << read_buffer << std::endl;
        std::string * read_args = split(read_buffer, '\n', 30);
        std::cout << split(read_args[0], ' ', 5)[0] << std::endl;

        if (split(read_args[0], ' ', 5)[0] == "GET") std::cout << "dziala!" << std::endl;*/

//GET /poland/population HTTP/1.0

        std::string * request_lines = split(read_buffer, '\n');//, 30);
        std::string * request_details = split(read_buffer, ' ');//, 3);
        ///std::cout << request_lines[0] << std::endl;
        //std::cout << request_lines[1] << std::endl;
        std::string send_buffer = "HTTP/1.0 "; //" 200 OK\nContent-Type: text/plain\nContent-Length: ";

        //std::cout << "PATH: " << request_details[0] << std::endl;
        //std::cout << "PATH: " << request_details[1] << std::endl;

        //std::cout << "Path: " << request_details[1] << std::endl;
        std::string * path = split(request_details[1], '/');//, 5);

        if (request_details[0] == HTTP_METHOD_GET)//"GET")
        {
          //std::cout << "PATH: " << path[0] << std::endl;
          //std::cout << "Path:" << request_details[1] << std::endl;
          //std::cout << "PATH: " << path[0] << " " << path[1] << std::endl;
          if (path[1] == "world")
          {
            std::cout << "PATHH: " << path[2] << std::endl;
            if (path[2] == "population")
            {
              send_buffer += "200 OK\nContent-Type: text/plain\nContent-Length: " + std::to_string(number_length(world->get_population())) + "\n\n" + std::to_string(world->get_population());
            }
            else if (path[2] == "infected")
            {
              send_buffer += "200 OK\nContent-Type: text/plain\nContent-Length: " + std::to_string(number_length(world->get_infected())) + "\n\n" + std::to_string(world->get_infected());
            }
            else
            {
              //return 404
            }
          }
          else if (request_details[1] == "/" || request_details[1] == "/favicon.ico")
          {
            send_buffer += "404 Not Found Error";
            //serve error
          }
          else
          {
            Country * country = world->get_country(path[1]);

            //If country not exist return 404 (Not Found)
            if (country == NULL)
            {
              //404
            }

            if (path[2] == "population")
            {
              send_buffer += "200 OK\nContent-Type: text/plain\nContent-Length: " + std::to_string(number_length(country->get_population())) + "\n\n" + std::to_string(country->get_population());
            }
            else if (path[2] == "population_density")
            {
              send_buffer += "200 OK\nContent-Type: text/plain\nContent-Length: " + std::to_string(number_length(country->get_population_density())) + "\n\n" + std::to_string(country->get_population_density());
            }
            else if (path[2] == "infected")
            {
              send_buffer += "200 OK\nContent-Type: text/plain\nContent-Length: " + std::to_string(number_length(country->get_infected())) + "\n\n" + std::to_string(country->get_infected());
            }
          }
        }
        else if (request_details[0] == HTTP_METHOD_POST)
        {
          if (path[1] == "system")
          {
            //Command to turn off the server
            if (path[2] == "off")
            {
              std::cout << "Turning off..." << std::endl;
              *keep_alive = false;
            }
            //Command to reset simulator
            else if (path[2] == "reset")
            {

            }
            //Uknown command
            else
            {
              //404
            }
          }
        }
        else
        {

        }

        delete[] request_lines;
        delete[] request_details;
        delete[] path;

        /*for (int i = 0; i < 30; i++)
        {
          std::cout << "line: " << request_lines[i] << std::endl;
        }*/

        //std::string send_buffer = "HTTP/1.0 200 OK\nContent-Type: text/plain\nContent-Length: ";//"Content-Length: 2\n\nxd";;


        //char send_buffer_x[] = "HTTP/1.0 200 OK\nContent-Type: text/plain\nContent-Length: 2\n\nxd";

        //ssize_t sockets_bytes_send = send(i, "HTTP/1.0 200 OK\nContent-Type: text/plain\nContent-Length: 2\n\nxd", strlen("HTTP/1.0 200 OK\nContent-Type: text/plain\nContent-Length: 2\n\nxd"), 0);
        ssize_t sockets_bytes_send = send(i, send_buffer.c_str(), send_buffer.length(), 0);

        //std::cout << "SEND: " << send_buffer << std::endl;

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
