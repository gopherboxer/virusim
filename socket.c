#include "socket.h"

#include <stdlib.h>

void listen_socket(struct sockaddr_in * socket_addr, int sockfd)
{
  int highest_sockfd = sockfd;
  fd_set sockets_read, sockets_write, sockets_error, sockets_copy;
  FD_ZERO(&sockets_read);
  FD_ZERO(&sockets_write);
  FD_ZERO(&sockets_error);
  FD_ZERO(&sockets_copy);
  FD_SET(sockfd, &sockets_copy);

  while (1)
  {
    sockets_read = sockets_copy;
    sockets_write = sockets_copy;
    sockets_error = sockets_copy;

    int socket_activity = select(highest_sockfd + 1, &sockets_read, &sockets_write, &sockets_error, NULL);

    if (socket_activity < 0)
    {
      puts("Error: Select.");
    }

    if (FD_ISSET(sockfd, &sockets_read))
    {
      unsigned int socket_len = 0;
      struct sockaddr_in new_socket;

      //Handle new connection
      int new_sockfd = accept(sockfd, (struct sockaddr*)&new_socket, &socket_len);

      if (new_sockfd < 0)
      {
        puts("Error: Accept new connection.");
      }

      if (new_sockfd > highest_sockfd) highest_sockfd = new_sockfd;

      FD_SET(new_sockfd, &sockets_copy);
    }

    for (int i = sockfd + 1; i <= highest_sockfd; i++)
    {

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

        char * send_buffer = (char *) malloc(strlen("HTTP/1.0 404 Not Found\n") * sizeof(char));
        char ** request_details = split(read_buffer, ' ');
        char ** path = split(request_details[1], '/');

        if (strncmp(path[1], "", 1) != 0) strcat(send_buffer, "HTTP/1.0 404 Not Found\n");
         //strcat(send_buffer, "HTTP/1.0 200 OK Content-Type: text/html Content-Length: 4\n\ndupa");
        {
          long index_file_length;
          char * index_file_content = get_file_content("index.html");//, &index_file_length);
          //strcat(send_buffer, "HTTP/1.0 200 OK Content-Type: text/html Content-Length: 4\n\ndupa");
          //strcat(send_buffer, "HTTP/1.0 200 OK\nContent-Type: text/html\n\n");
          //printf("INDEX_LENGTH: %ld\n", index_file_length);

          //send_buffer = realloc(send_buffer, )
          //strcat(send_buffer, "HTTP/1.0 200 OK\nContent-Type: text/html\n\n");
          //strcat(send_buffer, index_file_content);
          //printf("INDEX: %lu\n", strlen(index_file_content));

          //send_buffer = realloc(send_buffer, strlen(index_file_content) * strlen("HTTP/1.0 200 OK\nContent-Type: text/html\n\n") * sizeof(char));
          send_buffer = realloc(send_buffer, strlen(index_file_content) * strlen("HTTP/1.0 200 OK\nContent-Type: text/html\n\n") * sizeof(char));

          if (send_buffer == NULL) strcat(send_buffer, "HTTP/1.0 500 Internal Server\n");
          //char * send_buffer = malloc(send_buffer, (strlen(index_file_content) + strlen("HTTP/1.0 200 OK\nContent-Type: text/html\n\n") * sizeof(char));///malloc(1024 * sizeof(char));

          strcat(send_buffer, "HTTP/1.0 200 OK\nContent-Type: text/html\n\n");
          strcat(send_buffer, index_file_content);

          free(index_file_content);
        }
        /*(else
        {
          //long index_file_length;
          char * index_file_content = get_file_content("index.html");//, &index_file_length);
          //send_buffer = realloc(send_buffer, 1000 * sizeof(char));

          //if(send_buffer == NULL) printf("Error: Memory reallocation.\n");

          //send_buffer = malloc(index_file_length + 41 * sizeof(char));
          //send_buffer = malloc(1000 * sizeof(char));
          //free(&index_file_length);

          strcat(send_buffer, "HTTP/1.0 200 OK Content-Type: text/html\n\n");
          strcat(send_buffer, index_file_content);
          //free(index_file_content);
          //printf("TO SEND:%s", send_buffer);
        }*/

        ssize_t sockets_bytes_send = send(i, send_buffer, strlen(send_buffer), 0);

        free(send_buffer);

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
