// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#pragma once

#include <map>
#include <string>

// Runs until all events in the queue are processed.
void process_ui_events();

int run_main_window(
  std::map<std::string, std::string>* settings,
  int argc, char *argv[]);
