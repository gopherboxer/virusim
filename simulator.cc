#include "simulator.hh"

SimulatorTask::SimulatorTask(World * _world, bool * _keep_alive, bool * _waiting_for_necessary_data):
  world(_world),
  keep_alive(_keep_alive),
  waiting_for_necessary_data(_waiting_for_necessary_data)
{}

void SimulatorTask::run()
{
  //world->set_keep_alive(false);

  int susceptible_old = 0, infected_old = 0, recovered_old = 0, tries = 0;
  //std::cout << world->get_keep_alive() << std::endl;
  //while (world->get_keep_alive())
  while (*keep_alive)
  {
    //if (world->get_waiting_for_necessary_data()) continue;

    if (*waiting_for_necessary_data) continue;

    //simulate(world);
    //simulate(world);
    simulate();

    if (world->get_susceptible() == susceptible_old || world->get_infected() == infected_old || world->get_recovered() == recovered_old)
    {
      tries++;
    }
    else
    {
      susceptible_old = world->get_susceptible();
      infected_old = world->get_infected();
      recovered_old = world->get_recovered();
      tries = 0;
    }

    if (tries == 5)
    {
      //world->set_keep_alive(false);
      //*keep_alive = false;
      *waiting_for_necessary_data = true;
      tries = 0;
    }

    sleep(1);
  }
}

void SimulatorTask::simulate()
{
  unsigned int S = world->get_susceptible();
  unsigned int I = world->get_infected();
  unsigned int R = world->get_recovered();

  std::cout << " S: " << S << " I: " << I << " R: " << R << std::endl;
  
  float r = world->get_probability_of_infection();
  float a = world->get_probability_of_cure();

  int dS = r * S * I;
  int dR = a     * I;

  S -= dS;
  I += dS - dR;
  R += dR;

  world->set_susceptible(S);
  world->set_infected(I);
  world->set_recovered(R);
}
