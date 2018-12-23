#ifndef _YOA_VERSION_H
#define _YOA_VERSION_H

#include "Platform.h"

// version number set through CMake prejoect build process
#define YOA_VERSION_MAJOR 0
#define YOA_VERSION_MINOR 0

// toggle certain features of YoAudio
#define SDL2_ENABLED true
#define SDPLOG_ENABLED true

// calls to YoManager::Update() per second
#define FRAME_RATE 60;

// audio device configuration
#define AUDIO_FREQUENCY 48000
#define AUDIO_CHANNELS 2
#define AUDIO_SAMPLES 4096

#endif // _YOA_VERSION_H
