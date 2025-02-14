#pragma once

#include <math.h>
#include "LinearSmooothValue.h"

struct StereoPanner {
	LinearSmooothValue Pan;

	float volL = 0.77f;
	float volR = 0.77f;

	StereoPanner() noexcept {
		Pan.Reset(0.0f);
		Pan.SetFadeLength(500);
	}

	void Set(const float pan) {
		Pan.SetValue(pan);
	}

	void CalculateNext() {
		const float t = Pan.GetNext();
		volL = sqrt(0.5f * (1.0f - t));
		volR = sqrt(0.5f * (1.0f + t));
	}
};