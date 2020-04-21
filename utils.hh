#pragma once

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "world.hh"

bool is_file_exist(const char * path);
std::string * split(std::string s, char d);
std::string * split(std::string s, char d, size_t * tab_size);
int load_configuration(World * world, const char * path);
