#include <thread>
#include <iostream>
#include <sys/un.h>
#include <unistd.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include "socket.hh"
#include "simulator.hh"
#include "utils.hh"

#define MAX_SOCKET_QUEUE 10
#define FNULL 0.0

int main(int argc, const char * argv[])
{
  //Default values
  int socket_port = 5050;
  World * world = new World(FNULL, FNULL);
  bool keep_alive = true, waiting_for_necessary_data = false;

  if (argc > 1)
  {
    for (int i = 1; i < argc; i += 2)
    {
      if (strncmp(argv[i], "-port", 5) == 0 && argc-1 >= i+1) socket_port = atoi(argv[i + 1]);
      else if (strncmp(argv[i], "-pfc", 4) == 0 && argc-1 >= i+1) world->set_probability_of_cure(std::stof(argv[i + 1]));
      else if (strncmp(argv[i], "-pfi", 4) == 0 && argc-1 >= i+1) world->set_probability_of_infection(std::stof(argv[i + 1]));
      else if (strncmp(argv[i], "-p", 2) == 0 && argc-1 >= i+1) world->set_population(std::stoul(argv[i + 1]));
      else if (strncmp(argv[i], "-s", 2) == 0 && argc-1 >= i+1) world->set_susceptible(std::stoul(argv[i + 1]));
      else if (strncmp(argv[i], "-i", 2) == 0 && argc-1 >= i+1) world->set_infected(std::stoul(argv[i + 1]));
      else if (strncmp(argv[i], "-r", 2) == 0 && argc-1 >= i+1) world->set_recovered(std::stoul(argv[i + 1]));
      else
      {
        std::cout << "Usage: ./virusim -port <port> -pfc <probability_of_cure> -pfi <probability_of_infection> -p <population> -s <susceptible> -i <infected> -r <recovered>" << std::endl;
        return 1;
      }
    }
  }

  std::cout << "Listening on port: " << socket_port << std::endl;

  if (world->get_probability_of_cure() == FNULL || world->get_probability_of_infection() == FNULL)
  {
    waiting_for_necessary_data = true;
    std::cout << "You didn't set the probability_of_cure or the probability_of_infection!!!\nProgram is waiting for this data!!!" << std::endl;
  }

  world->set_probability_of_infection(world->get_probability_of_infection()/world->get_population());

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
  {
    std::cout << "Error: Creating socket" << std::endl;
    return 1;
  }

  struct sockaddr_in socket;
  socket.sin_family = AF_INET;
  socket.sin_addr.s_addr = INADDR_ANY;
  socket.sin_port = htons(socket_port);

  int opt = 1;
  int error = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

  if (error == 0)
  {
    std::cout << "Error: Setting socket." << std::endl;
    return 1;
  }

  error = bind(sockfd, (struct sockaddr*)&socket, sizeof(socket));

  if (error < 0)
  {
    std::cout << "Error: Binding socket." << std::endl;
    return 1;
  }

  SimulatorTask simulator(world, &keep_alive, &waiting_for_necessary_data);

  std::thread calculation_thread(&SimulatorTask::run, &simulator);//, world);

  error = listen(sockfd, MAX_SOCKET_QUEUE);

  if (error != 0)
  {
    std::cout << "Error: Starting listen." << std::endl;
    return 1;
  }

  listen_socket(&socket, sockfd, world, &keep_alive, &waiting_for_necessary_data);

  close(sockfd);

  return 0;
}
