#pragma once


#include "TMeasuredValue.h"
#include <vector>
#include "Peak.h"
#include "SegmentDays.h"

namespace  PeakDetector
{

	class PeakDetector
	{
	public:
		PeakDetector(bool parralelism);
		~PeakDetector();
		void detectPeaks(Common::SegmentDays *days, int * windowSize, std::vector<std::vector<PeakPeakDetector::Peak>> *detectedPeaks);

	private:
		double calculateWindowFitness(std::vector<Common::TMeasuredValue *> *data, size_t startIndex, size_t endIndex);
		void detectPeakInData(std::vector<Common::TMeasuredValue *> *data, std::vector<PeakPeakDetector::Peak> *peaks, int * windowSize);
		bool paralelism;
	};

}

