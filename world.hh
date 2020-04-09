#pragma once

#include <map>
#include "country.hh"

class World
{
  private:
    std::map<std::string, Country*> countries;
  public:
    void push_country(Country * country);
    Country * get_country(std::string country_name);
    unsigned int get_population();
    unsigned int get_infected();
    std::map<std::string, Country *> get_countries();
};
