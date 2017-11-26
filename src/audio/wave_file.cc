// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include "audio/wave_file.h"

#include <stdio.h>

#include "utils/log.h"

namespace {

uint16_t read16(uint8_t *data) {
  return (uint16_t)data[0] | ((uint16_t)data[1] << 8);
}

uint32_t read32(uint8_t *data) {
  return (uint32_t)data[0] | ((uint32_t)data[1] << 8) |
         ((uint32_t)data[2] << 16) |((uint32_t)data[3] << 24);
}

}  // namespace

WaveFile::WaveFile()
    : format_(0), channels_(0), sample_rate_(0),
      bits_per_sample_(0), data_size_(0),
      data_(NULL), shift_(0) {
}

WaveFile::~WaveFile() {
  cleanup();
}

bool WaveFile::read(const char *name) {
  FILE *f = fopen(name, "rb");

  if (!f) {
    Log::printf(Log::LERROR, "Failed to open %s\n", name);
    return false;
  }

  if (fseek(f, 0, SEEK_END)) {
    fclose(f);
    Log::printf(Log::LERROR, "Failed to seek %s\n", name);
    return false;
  }

  int32_t sz = ftell(f);

  if (sz <= 0) {
    fclose(f);
    Log::printf(Log::LERROR, "Failed to get size %s\n", name);
    return false;
  }

  fseek(f, 0, SEEK_SET);

  data_ = new uint8_t[sz];
  fread(data_, sz, 1, f);
  fclose(f);

  const uint32_t kRiff = 0x46464952;  // "RIFF"
  const uint32_t kWave = 0x45564157;  // "WAVE"
  const uint32_t kFormat = 0x20746d66;  // "fmt "
  const uint32_t kData = 0x61746164;  // "data"

  if (sz < 40) {
    cleanup();
    Log::printf(Log::LERROR, "File %s too small\n", name);
    return false;
  }

  if (read32(data_) != kRiff ||
      read32(data_ + 8) != kWave ||
      read32(data_ + 12) != kFormat) {
    cleanup();
    Log::printf(Log::LERROR, "Not a wave file:%08X %08X %08X\n",
                read32(data_), read32(data_ + 8), read32(data_ + 12));
    return false;
  }

  format_ = read16(data_ + 20);
  channels_ = read16(data_ + 22);
  sample_rate_ = read32(data_ + 24);
  bits_per_sample_ = read16(data_ + 34);

  int data_chunk = 20 + read32(data_ + 16);
  if (read32(data_ + data_chunk) != kData) {
    cleanup();
    Log::printf(Log::LERROR, "Data stream not found:%08X\n",
                read32(data_ + data_chunk));
    return false;
  }

  data_size_ = read32(data_ + data_chunk + 4);
  shift_ = data_chunk + 8;
  return true;
}

void WaveFile::cleanup() {
  delete[] data_;
  data_ = NULL;
  data_size_ = 0;
}
