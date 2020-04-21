#pragma once

class World
{
  private:
    float probability_of_cure, probability_of_infection;
    unsigned int population, susceptible, infected, recovered;
  public:
    World(float _probability_of_cure, float _probability_of_infection);

    float get_probability_of_cure();
    float get_probability_of_infection();

    unsigned int get_population();
    unsigned int get_susceptible();
    unsigned int get_infected();
    unsigned int get_recovered();

    void set_probability_of_cure(float _probability_of_cure);
    void set_probability_of_infection(float _probability_of_infection);

    void set_population(unsigned int _population);
    void set_susceptible(unsigned int _susceptible);
    void set_infected(unsigned int _infected);
    void set_recovered(unsigned int _recovered);
};
