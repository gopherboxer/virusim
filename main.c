#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "socket.h"

#define MAX_SOCKET_QUEUE 10

int main(int argc, const char * argv[])
{
  //Default values
  int socket_port = 5050;

  if (argc > 1)
  {
    for (int i = 1; i < argc; i += 2)
    {
      if (strncmp(argv[i], "-port", 5) == 0 && argc-1 >= i+1) socket_port = atoi(argv[i + 1]);
      else
      {
        puts("Usage: ./virusim -port <port>");
        return 1;
      }
    }
  }

  printf("Listening on port: %d\n", socket_port);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
  {
    puts("Error: Creating socket");
    return 1;
  }

  struct sockaddr_in socket;
  socket.sin_family = AF_INET;
  socket.sin_addr.s_addr = INADDR_ANY;
  socket.sin_port = htons(socket_port);

  int opt = 1;
  int error = setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

  if (error < 0)
  {
    puts("Error: Setting socket.");
    return 1;
  }

  error = bind(sockfd, (struct sockaddr*)&socket, sizeof(socket));

  if (error < 0)
  {
    puts("Error: Binding socket.");
    return 1;
  }

  error = listen(sockfd, MAX_SOCKET_QUEUE);

  if (error != 0)
  {
    puts("Error: Starting listen.");
    return 1;
  }

  listen_socket(&socket, sockfd);

  close(sockfd);

  return 0;
}
