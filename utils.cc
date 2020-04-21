#include "utils.hh"

bool is_file_exist(const char * path)
{
  FILE * file = fopen(path, "r");

  if (file)
  {
    fclose(file);
    return file;
  }
  return file;
}

std::string * split(std::string s, char d)
{
  size_t tab_size = 0;

  for (int i = 0; i < s.length(); i++)
  {
    if (s[i] == d) tab_size++;
  }

  std::string * result = new std::string[tab_size + 1];
  int index = 0;

  for (int i = 0; i < s.length(); i++)
  {
    if (s[i] == d)
    {
      index++;
    }
    else
    {
      result[index] += s[i];
    }
  }
  return result;
}

std::string * split(std::string s, char d, size_t * tab_size)
{
  *tab_size = 0;
  for (int i = 0; i < s.length(); i++)
  {
    if (s[i] == d) *tab_size += 1;
  }
  *tab_size += 1;
  std::string * result = new std::string[*tab_size];
  int index = 0;

  for (int i = 0; i < s.length(); i++)
  {
    if (s[i] == d)
    {
      index++;
    }
    else
    {
      result[index] += s[i];
    }
  }
  return result;
}

int load_configuration(World * world, const char * path)
{
  std::string line;
  std::ifstream configuration_file(path);

  if (!configuration_file.good()) return 1;

  while (std::getline(configuration_file, line))
  {
    std::string * line_data = split(line, ' ');

    //world->set
    //Country * country = new Country(line_data[0], std::stoul(line_data[1], nullptr, 0), std::stoul(line_data[2], nullptr, 0), std::stoul(line_data[3], nullptr, 0));
    //world->push_country(country);
  }

  configuration_file.close();
  return 0;
}
