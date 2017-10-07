// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include "utils/exec_with_return.h"

#include <iostream>
#include <memory>
#include <string>
#include <array>

std::string exec_with_return(const char *command) {
  const unsigned kBuffSize = 1024;
  std::array<char, kBuffSize> buff;
  std::string res;
  std::shared_ptr<FILE> pipe(popen(command, "r"), pclose);
  if (!pipe)
    return res;
  while (!feof(pipe.get()))
    if (fgets(buff.data(), kBuffSize, pipe.get()) != NULL)
      res.append(buff.data());
  return res;
}
