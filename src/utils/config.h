// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#pragma once

#include <map>
#include <string>

// Reads and writes file in the form
// <KEY>[=[<VALUE>]]
std::map<std::string, std::string> read_config_file(const char *name);
void write_config_file(
    const char *name, const std::map<std::string, std::string>& config);

