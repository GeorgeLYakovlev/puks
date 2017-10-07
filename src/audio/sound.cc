// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include "audio/sound.h"

#include "utils/log.h"

Sound::Sound(const char *device)
    : channels_(0), sample_rate_(0), bits_per_sample_(0) {
  int error;
  params_ = NULL;
  if ((error = snd_pcm_open(&handle_, device,
                            SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    Log::printf(Log::LERROR, "Failed to open \"%s\" with error:%s\n",
                device, snd_strerror(error));
    handle_ = NULL;
  } else {
    snd_pcm_hw_params_malloc(&params_);
    snd_pcm_hw_params_any(handle_, params_);
    if ((error = snd_pcm_hw_params_set_access(
           handle_, params_, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
      Log::printf(Log::LERROR, "Failed to set interleaved mode with error:%s\n",
             snd_strerror(error));
    }
  }
}

Sound::~Sound() {
  if (params_) {
    snd_pcm_hw_params_free(params_);
  }
  if (handle_) {
    snd_pcm_drain(handle_);
    snd_pcm_close(handle_);
  }
}

bool Sound::play_wave_file(const WaveFile& f) {
  int error;
  if (f.bits_per_sample() != bits_per_sample_) {
    // We support only LE wave streams.
    snd_pcm_format_t bps = SND_PCM_FORMAT_UNKNOWN;
    switch (bits_per_sample_) {
    case 8: bps = SND_PCM_FORMAT_S8; break;
    case 16: bps = SND_PCM_FORMAT_S16_LE; break;
    case 24: bps = SND_PCM_FORMAT_S24_LE; break;
    case 32: bps = SND_PCM_FORMAT_S32_LE; break;
    }
    if (bps != SND_PCM_FORMAT_UNKNOWN) {
      if ((error = snd_pcm_hw_params_set_format(handle_, params_, bps)) < 0) {
        Log::printf(Log::LERROR,
                    "Failed to set bits per sample with error:%s\n",
                    snd_strerror(error));
        return false;
      }
    }
    bits_per_sample_ = f.bits_per_sample();
  }
  if (f.channels() != channels_) {
    if ((error = snd_pcm_hw_params_set_channels(
            handle_, params_, f.channels())) < 0) {
      Log::printf(Log::LERROR, "Failed to set channels with error:%s\n",
                  snd_strerror(error));
      return false;
    }
    channels_ = f.channels();
  }
  if (f.sample_rate() != sample_rate_) {
    sample_rate_ = f.sample_rate();
    if ((error = snd_pcm_hw_params_set_rate_near(
            handle_, params_, &sample_rate_, NULL)) < 0) {
      Log::printf(Log::LERROR, "Failed to set sample rate with error:%s\n",
                  snd_strerror(error));
      return false;
    }
  }
  if ((error = snd_pcm_hw_params(handle_, params_)) < 0) {
    printf("Failed to set parameters with error:%s\n",
           snd_strerror(error));
    return false;
  }
  // Write sound
  int frames = static_cast<int>((f.data_size() / f.channels()) /
                                (f.bits_per_sample() / 8));
  if ((error = snd_pcm_writei(handle_, f.data(), frames)) < 0) {
    Log::printf(Log::LERROR, "Failed to write data:%d\n", error);
    return false;
  }
  return true;
}
