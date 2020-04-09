#include "world.hh"

void World::push_country(Country * country)
{
  countries[country->get_name()] = country;
}

Country * World::get_country(std::string country_name)
{
  std::map<std::string, Country *>::iterator it;
  it = countries.find(country_name);

  if (it != countries.end()) return it->second;

  return NULL;
}

unsigned int World::get_population()
{
  std::map<std::string, Country *>::iterator it;
  unsigned int counter = 0;
  for (it = countries.begin(); it != countries.end(); it++ )
  {
    counter += it->second->get_population();
  }
  return counter;
}

unsigned int World::get_infected()
{
  std::map<std::string, Country *>::iterator it;
  unsigned int counter = 0;
  for (it = countries.begin(); it != countries.end(); it++ )
  {
    counter += it->second->get_infected();
  }
  return counter;
}

std::map<std::string, Country *> World::get_countries()
{
  return countries;
}
