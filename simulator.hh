#pragma once

#include "unistd.h"

#include "world.hh"
#include "medicine.hh"

class SimulatorTask
{
  private:
    World * world;
    bool * keep_alive;
  public:
    SimulatorTask(World * _world, bool * _keep_alive);
    void run();
};
//void simulate(World world);

//void simulate(/*World & world*/);//, Medicine * medicine);
