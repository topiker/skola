#include "Peak.h"

namespace PeakPeakDetector {

	Peak::Peak(unsigned int startIndex, unsigned int endIndex, double fitness)
	{
		this->startIndex = startIndex;
		this->endIndex = endIndex;
		this->fitness = fitness;
	}
}
