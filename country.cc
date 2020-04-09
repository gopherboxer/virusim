#include "country.hh"

Country::Country(void) {}

Country::Country(std::string _name, unsigned int _population, unsigned int _population_density)
{
  name = _name;
  population = _population;
  population_density = _population_density;
}

Country::Country(std::string _name, unsigned int _population, unsigned int _population_density, unsigned int _infected)
{
  name = _name;
  population = _population;
  population_density = _population_density;
  infected = _infected;
}

std::string Country::get_name()
{
  return name;
}

unsigned int Country::get_population()
{
  return population;
}

unsigned int Country::get_population_density()
{
  return population_density;
}

unsigned int Country::get_infected()
{
  return infected;
}

void Country::set_infected(unsigned int _infected)
{
  infected = _infected;
}
