#include "world.hh"

World::World(float _probability_of_cure, float _probability_of_infection):
  probability_of_cure(_probability_of_cure),
  probability_of_infection(_probability_of_infection/38000000),
  population(38000000),
  infected(10),
  recovered(0)
{
  susceptible = population - infected;
}

float World::get_probability_of_cure()
{
  return probability_of_cure;
}

float World::get_probability_of_infection()
{
  return probability_of_infection;
}

unsigned int World::get_population()
{
  return population;
}

unsigned int World::get_susceptible()
{
  return susceptible;
}

unsigned int World::get_infected()
{
  return infected;
}
unsigned int World::get_recovered()
{
  return recovered;
}

void World::set_probability_of_cure(float _probability_of_cure)
{
  probability_of_cure = _probability_of_cure;
}
void World::set_probability_of_infection(float _probability_of_infection)
{
  probability_of_infection = _probability_of_infection;
}

void World::set_population(unsigned int _population)
{
  population = _population;
}

void World::set_susceptible(unsigned int _susceptible)
{
  susceptible = _susceptible;
}

void World::set_infected(unsigned int _infected)
{
  infected = _infected;
}

void World::set_recovered(unsigned int _recovered)
{
  recovered = _recovered;
}
