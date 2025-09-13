#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <math.h>

#include "LinearSmooothValue.h"

namespace YoaEngine
{
	class SineSample
	{
	private:
		LinearSmooothValue frequency;   // Frequency of the sine wave
		LinearSmooothValue amplitude;   // Amplitude of the sine wave
		unsigned int sampleRate;        // Sample rate in Hz
		float phase;                    // Current phase of the sine wave
		float increment;                // Phase increment per sample

    public:
		const uint32_t ID;
    public:
		SineSample(uint32_t id, const float toneFrequency, const unsigned int sampleRate, const float toneAmplitude)
		    : ID(id), sampleRate(sampleRate), phase(0.0f)
	    {
			frequency.Reset(toneFrequency);
			frequency.SetFadeLength(MIN_FADE_LENGTH * TARGET_SAMPLERATE);
			amplitude.Reset(0.0f);
			SetAmplitude(toneAmplitude);

		    increment = 2.0f * M_PI * toneFrequency / sampleRate;
	    }

	    float GetSample(const unsigned int position)
	    {
			if (amplitude.HasReachedTarget() && amplitude.GetCurrent() == 0.0f)
			{
				return 0.0f;
			}

            if (frequency.HasReachedTarget() == false)
            {
				auto newFrequency = frequency.GetNext();
				increment = 2.0f * M_PI * newFrequency / sampleRate;
            }

		    // Calculate the sample value at the given position
		    float sample = sin(phase) * amplitude.GetNext();
		    phase += increment;

		    // Wrap phase to stay within the range of 0 to 2π
		    if (phase >= 2.0f * M_PI)
		    {
			    phase -= 2.0f * M_PI;
		    }

		    return sample;
	    }

	    void SetFrequency(float newFrequency, const float fadeTime = 0.0f)
	    {
			newFrequency = std::max(0.0f, newFrequency);
			newFrequency = std::min(newFrequency, sampleRate / 2.0f);

            if (newFrequency != frequency.GetCurrent() && frequency.HasReachedTarget())
            {
				return;
            }

			frequency.SetValue(newFrequency);
			increment = 2.0f * M_PI * newFrequency / sampleRate;
	    }
		void SetAmplitude(float newAmplitude, const float fadeOut = -1.0f)
		{
			newAmplitude = std::max(0.0f, newAmplitude);
			newAmplitude = std::min(newAmplitude, 1.0f);

			if (newAmplitude == amplitude.GetCurrent() && amplitude.HasReachedTarget())
			{
				return;
			}

            if (fadeOut >= 0.0f)
			{
				amplitude.SetFadeLength(fadeOut * sampleRate);
			}
            else
            {
				amplitude.SetFadeLength(MIN_FADE_LENGTH * sampleRate);
            }
			amplitude.SetValue(newAmplitude);
			amplitude.UpdateTarget();
		}
        bool HasStopped()
        {
			return amplitude.HasReachedTarget() && amplitude.GetCurrent() == 0.0f;
        }
    };
} // namespace YoaEngine
