#pragma once

#include <iostream>

class Country
{
  private:
    std::string name;
    unsigned int population, population_density, infected;// = 0;
    bool closedborder /*= false*/, isolated; //= false;
  public:
    Country();
    Country(std::string _name, unsigned int _population, unsigned int _population_density);
    Country(std::string _name, unsigned int _population, unsigned int _population_density, unsigned int _infected);

    std::string get_name();
    unsigned int get_population();
    unsigned int get_population_density();
    unsigned int get_infected();
    void set_infected(unsigned int _infected);
};
