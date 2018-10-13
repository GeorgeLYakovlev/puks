// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#pragma once

#include "audio/wave_file.h"

enum ESOUND_FILES {
  ESOUND_CLICK0,
  ESOUND_CLICK1,
  ESOUND_CLICK2,
  ESOUND_CLICK3,
  ESOUND_CLICK4,
  ESOUND_CLICK5,
  ESOUND_CLICK6,
  ESOUND_CLICK7,
  ESOUND_CLICK8,
  ESOUND_BEEP,
  ESOUND_BUMMER,
  ESOUND_1SEC,
  ESOUND_EXPIRE,
  ESOUND_FALSESTART,
  ESOUND_GONG,
  ESOUND_HAMMER,
  ESOUND_HAMMER0,
  ESOUND_HAMMER1,
  ESOUND_NEWTOPIC,

  // Maximal value for the sound ID, not an actual sound.
  ESOUND_MAX
};

const WaveFile& getWaveFile(ESOUND_FILES file);
