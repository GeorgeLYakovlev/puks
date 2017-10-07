// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include "utils/config.h"
#include <cstring>

std::map<std::string, std::string> read_config_file(const char *name) {
  std::map<std::string, std::string> res;
  FILE *f = fopen(name, "rt");
  if (!f)
    return res;
  char buff[1024];
  while (fgets(buff, sizeof(buff), f)) {
    char *p = strchr(buff, '\n');
    if (p) *p = 0;
    p = strchr(buff, '\r');
    if (p) *p = 0;
    p = strchr(buff, '=');
    if (p) {
      *p = 0;
      ++p;
    } else {
      p = buff + strlen(buff);
    }
    res[buff] = p;
  }
  fclose(f);
  return res;
}

void write_config_file(
    const char *name,
    const std::map<std::string,
    std::string>& config) {
  FILE *f = fopen(name, "wt");
  if (!f)
    return;

  for (auto it = config.cbegin(); it != config.cend(); ++it) {
    fprintf(f, "%s=%s\n", it->first.c_str(), it->second.c_str());
  }

  fclose(f);
}
