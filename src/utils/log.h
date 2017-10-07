// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#pragma once

class Log {
 public:

  enum Level {
    LNONE,
    LERROR,
    LWARNING,
    LINFO,
    LDEBUG
  };

  static void set_level(Level l);

  static void printf(Level l, const char *format, ...);

 private:
  explicit Log(Level print_level) : level_(print_level) {}

  Level level_;

  static Log log_;
};
