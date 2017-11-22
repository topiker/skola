#pragma once


#include "TMeasuredValue.h"
#include <vector>
#include "Peak.h"
#include "SegmentDays.h"

namespace  PeakDetector
{

		void detectPeaks( Common::SegmentDays*const days, int * windowSize, std::shared_ptr< std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& detectedPeaks, bool paralelismPerDay);
		double calculateWindowFitness(std::vector<std::unique_ptr<Common::TMeasuredValue>>*const data, size_t startIndex, size_t endIndex);
		void detectPeakInData(std::vector<std::unique_ptr<Common::TMeasuredValue>>*const data, std::shared_ptr<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>& peaks, int * windowSize);
}

