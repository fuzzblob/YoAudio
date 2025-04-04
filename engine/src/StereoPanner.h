#pragma once

#include "EngineConfig.h"
#include "LinearSmooothValue.h"

#include <math.h>

namespace YoaEngine
{
	struct StereoPanner {
		LinearSmooothValue Pan;

		// TODO(maris): check is this to do with pan law?
		static constexpr float centerPan = 0.75f;
		float volL = centerPan;
		float volR = centerPan;

		StereoPanner() noexcept {
			Pan.Reset(0.0f);
			Pan.SetFadeLength(MIN_FADE_LENGTH * TARGET_SAMPLERATE);
		}

		void Set(const float pan) {
			Pan.SetValue(pan);
		}

		void CalculateNext() {
			const float t = Pan.GetNext();
			volL = 1 - pow(0.5 * (1 + t), 2); // sqrt(0.5f * (1.0f - t));
			volR = 1 - pow(0.5 * (1 - t), 2); // sqrt(0.5f * (1.0f + t));
		}

		float ReturnCurrentPan(){
			return Pan.GetCurrent();
		}
	};
}  // namespace YoaEngine
