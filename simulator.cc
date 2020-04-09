#include "simulator.hh"

//void simulate(/*World & world*/)//, Medicine * medicine)
//{
  //std::cout << "World thread: " /*<< world.get_population()*/ << std::endl;
//}

SimulatorTask::SimulatorTask(World * _world, bool * _keep_alive)
{
  world = _world;
  keep_alive = _keep_alive;
}

void SimulatorTask::run()
{
  while (*keep_alive)
  {
    /*std::cout << "POPULATION: " << world->get_population() << std::endl;
    std::cout << "INFECTED: " << world->get_infected() << std::endl;
    std::cout << "--------------------" << std::endl;
*/
    std::map<std::string, Country * >::iterator it = world->get_countries().begin();

    for (std::pair<std::string, Country * > country_element : world->get_countries())
    {
  		// Accessing KEY from element
  		/*std::string word = element.first;
  		// Accessing VALUE from element.
  		int count = element.second;
  		std::cout << word << " :: " << count << std::endl;*/
      unsigned int infected = country_element.second->get_infected();
      //std::cout << country_element.first << ": " << infected << std::endl;

      //std::cout << "NAME: " << country_element.second->get_name() << std::endl;

      /*
        --Algorithm--

        infected * infected/100

      */


      if (infected > 0 && infected < 1000)
      {
        infected++;
      }

      //If anyone don't alive
      if (world->get_population() == 0)
      {
        //Turning off the server
        *keep_alive = false;
        break;
      }

      /*if (infected > 0 && infected < 1000)
      {
        //std::cout << "NAME: " << country_element.second->get_name() << std::endl;
        country_element.second->set_infected(infected + 1);
      }*/

      //std::cout << "country: " << countries_element.first << std::endl;


  	}

    sleep(1);
  }
}
