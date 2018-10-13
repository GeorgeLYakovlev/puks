// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include "audio/sound_files.h"

#include <string>

#include "utils/log.h"

namespace {
std::string res(const char* file) {
  std::string s("resources/wave");
  s.append(file);
  return s;
}
}  // namespace

const WaveFile& getWaveFile(ESOUND_FILES file) {
  static WaveFile sound_files[ESOUND_MAX];
  static bool initialized = false;

  if (!initialized) {
    sound_files[ESOUND_CLICK0].read(res("click0.wav").c_str());
    sound_files[ESOUND_CLICK1].read(res("click1.wav").c_str());
    sound_files[ESOUND_CLICK2].read(res("click2.wav").c_str());
    sound_files[ESOUND_CLICK3].read(res("click3.wav").c_str());
    sound_files[ESOUND_CLICK4].read(res("click4.wav").c_str());
    sound_files[ESOUND_CLICK5].read(res("click5.wav").c_str());
    sound_files[ESOUND_CLICK6].read(res("click6.wav").c_str());
    sound_files[ESOUND_CLICK7].read(res("click7.wav").c_str());
    sound_files[ESOUND_CLICK8].read(res("click8.wav").c_str());
    sound_files[ESOUND_BEEP].read(res("beep.wav").c_str());
    sound_files[ESOUND_BUMMER].read(res("bummer.wav").c_str());
    sound_files[ESOUND_1SEC].read(res("1sec.wav").c_str());
    sound_files[ESOUND_EXPIRE].read(res("expire.wav").c_str());
    sound_files[ESOUND_FALSESTART].read(res("falsestart.wav").c_str());
    sound_files[ESOUND_GONG].read(res("gong.wav").c_str());
    sound_files[ESOUND_HAMMER].read(res("hammer.wav").c_str());
    sound_files[ESOUND_HAMMER0].read(res("hammer0.wav").c_str());
    sound_files[ESOUND_HAMMER1].read(res("hammer1.wav").c_str());
    sound_files[ESOUND_NEWTOPIC].read(res("newtopic.wav").c_str());
    initialized = true;
  }

  if (file >= sizeof(sound_files))
    return sound_files[ESOUND_BEEP];
  else
    return sound_files[file];
}
