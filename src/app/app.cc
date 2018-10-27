// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include <cstdio>
#include <map>
#include <string>

#include "ui/main_window.h"
#include "utils/config.h"
#include "utils/log.h"
#include "./version.h"

const char *kConfigFile = "puks.conf";

int main(int argc, char *argv[]) {
  std::map<std::string, std::string> settings = read_config_file(kConfigFile);
  printf("%s\nCopyright Palo Alto What?Where?When? club 2017\n",
         VERSION_STRING);
  Log::set_level(Log::LDEBUG);
  run_main_window(&settings, argc, argv);
  write_config_file(kConfigFile, settings);
}
