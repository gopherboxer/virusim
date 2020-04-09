#include <iostream>
#include <unistd.h>
//#include <pthread.h>
#include <thread>
#include <sys/un.h>
#include <sys/socket.h>
//#include <filesystem>

#include "socket.hh"
#include "simulator.hh"

#define MAX_SOCKET_QUEUE 10

int main(int argc, char * argv[])
{
  std::string custom_socket_path = "virusim.sock";

  if (argc > 1)
  {
    if (strncmp(argv[1], "-path", 5) == 0 && argv[2] != NULL)
    {
      custom_socket_path = argv[2];
    }
    else
    {
      std::cout << "Usage: ./virusim -path <path>" << std::endl;
      return 1;
    }
  }

  int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

  if (sockfd < 0)
  {
    std::cout << "Error: Creating socket" << std::endl;
    return 1;
  }

  struct sockaddr_un socket;
  socket.sun_family = AF_UNIX;

  strcpy(socket.sun_path, custom_socket_path.c_str());

  int error = unlink(socket.sun_path);

  if (error < 0 && is_file_exist(socket.sun_path))
  {
    std::cout << "Error: Unlinking." << std::endl;
    return 1;
  }

  error = bind(sockfd, (struct sockaddr*)&socket, sizeof(socket));

  if (error < 0)
  {
    std::cout << "Error: Binding socket." << std::endl;
    return 1;
  }

  //pthread_t calculation_thread;
  World * world = new World();
  Country * china_country = new Country("china", 138600000, 146, 1);
  Country * poland_country = new Country("poland", 38000000, 123, 0);
  Country * ukraine_country = new Country("ukraine", 42000000, 73, 0);
  //Country * country;
  world->push_country(china_country);
  world->push_country(poland_country);
  world->push_country(ukraine_country);

  bool keep_alive = true;
  SimulatorTask simulator(world, &keep_alive); //= new SimulatorTask(world);

  std::thread calculation_thread(&SimulatorTask::run, &simulator);//, world);

  error = listen(sockfd, MAX_SOCKET_QUEUE);

  if (error != 0)
  {
    std::cout << "Error: Starting listen." << std::endl;
    return 1;
  }

  listen_socket(&socket, sockfd, world, &keep_alive);

  close(sockfd);

  return 0;
}
