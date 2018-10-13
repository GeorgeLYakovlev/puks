// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#pragma once

#include <alsa/asoundlib.h>

#include "audio/wave_file.h"

class Sound {
 public:
  explicit Sound(const char *device = "default");
  ~Sound();

  bool play_wave_file(const WaveFile& f);

 private:
  snd_pcm_t *handle_;
  snd_pcm_hw_params_t *params_;

  uint16_t channels_;
  uint32_t sample_rate_;
  uint16_t bits_per_sample_;
};
