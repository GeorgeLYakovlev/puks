// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include "utils/log.h"

#include <stdio.h>
#include <stdarg.h>

Log Log::log_(Log::LINFO);

void Log::set_level(Log::Level l) {
  log_.level_ = l;
}

void Log::printf(Log::Level l, const char *format, ...) {
  if (l > log_.level_)
    return;

  char buffer[256];

  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  ::printf("%s", buffer);
  va_end(args);
}

