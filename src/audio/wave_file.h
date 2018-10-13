// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#pragma once

#include <stdint.h>
#include <stdio.h>

class WaveFile {
 public:
  WaveFile();
  ~WaveFile();

  bool read(const char *f);

  uint16_t format() const { return format_; }
  uint16_t channels() const { return channels_; }
  uint32_t sample_rate() const { return sample_rate_; }
  uint16_t bits_per_sample() const { return bits_per_sample_; }

  uint32_t data_size() const { return data_size_; }
  const uint8_t *data() const { return data_ ? data_ + shift_ : NULL; }

 private:

  void cleanup();

  uint16_t format_;
  uint16_t channels_;
  uint32_t sample_rate_;
  uint16_t bits_per_sample_;

  uint32_t data_size_;
  uint8_t *data_;
  int shift_;
};
