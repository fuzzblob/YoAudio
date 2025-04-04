#pragma once

#include "AudioFormat.h"

#include <cstdint>

namespace YoaEngine
{
	struct Sample {
		// sample data
		uint8_t* Buffer;
		// length of one channle of audio
		uint32_t Samples;
		// sample rate of the sample
		uint32_t Frequency;
		// sample format (used to convers buffer to float for mixing)
		SampleFormat Format;
		// amaount of channels present in the buffer
		uint8_t Channels;

		float GetSample(const uint32_t position) noexcept {
			switch (Format) {
			case YOA_Format_Float:
				return ((float*)Buffer)[position];
			case YOA_Format_Uint8:
				return (float)((uint8_t*)Buffer)[position] / 128.0f - 1.0f;
			case YOA_Format_Sint16:
				return (float)((int16_t*)Buffer)[position] / 32768.0f;
			case YOA_Format_Sint32:
				return (float)((int32_t*)Buffer)[position] / 2147483648.0f;
			case YOA_Format_Unknown:
			default:
				return 0.0f;
			}
		}
	};
}  // namespace YoaEngine
