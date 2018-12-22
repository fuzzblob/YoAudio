#ifndef _AUDIODEVICE_H
#define _AUDIODEVICE_H

#include "Platform.h"

struct AudioDevice
{
	SDL_AudioDeviceID DeviceID;
	SDL_AudioSpec SpecWanted;
	SDL_AudioSpec SpecObtained;
};

#endif // !_AUDIODEVICE_H
