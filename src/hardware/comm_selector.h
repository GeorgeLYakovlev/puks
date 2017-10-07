// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <vector>

class CommPort {
 public:
  explicit CommPort(const char* name)
    : comm_name_(name), speed_(115200), bits_(8) {
  }

  std::string name() const { return comm_name_; }
  uint32_t speed() const { return speed_; }
  uint32_t bits() const { return bits_; }

  CommPort& set_speed(uint32_t speed) {
    speed_ = speed;
    return *this;
  }

  CommPort& set_bits(uint32_t bits) {
    bits_ = bits;
    return *this;
  }

 private:
  std::string comm_name_;
  uint32_t speed_;
  uint32_t bits_;
};

class CommSelector {
 public:
  CommSelector();
  ~CommSelector();

  std::vector<CommPort> get_comms();
};

