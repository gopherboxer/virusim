#pragma once

#include <iostream>
#include "unistd.h"

#include "world.hh"

class SimulatorTask
{
  private:
    World * world;
    bool * keep_alive;
    bool * waiting_for_necessary_data;
  public:
    SimulatorTask(World * _world, bool * _keep_alive, bool * _waiting_for_necessary_data);
    void run();
    void simulate();
};
