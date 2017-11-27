#pragma once


#include "TMeasuredValue.h"
#include <vector>
#include "Peak.h"
#include "SegmentDays.h"
#include <amp.h>

namespace  PeakDetector
{
		void detectPeaks(Common::SegmentDays*const days, int * windowSize, std::shared_ptr< std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& detectedPeaks, bool paralelismPerDay);
		double calculateWindowFitness(std::vector<std::unique_ptr<Common::TMeasuredValue>>* const data, size_t startIndex, size_t endIndex);
		void detectPeakInData(std::vector<std::unique_ptr<Common::TMeasuredValue>>*const data, std::shared_ptr<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>& peaks, int * windowSize);
		std::vector<double> getFitnessOfAllWindows(std::vector<std::unique_ptr<Common::TMeasuredValue>>* const data, int * windowSize);
		double getAvgFitnessValue(std::vector<double>*avg);
		std::vector<std::shared_ptr<PeakPeakDetector::Peak>> joinOverlayedPeaks(std::vector<std::shared_ptr<PeakPeakDetector::Peak>>* peaksToJoin);
		void recalculatePeaksFitness(std::vector<std::unique_ptr<Common::TMeasuredValue>>*const data, std::vector<std::shared_ptr<PeakPeakDetector::Peak>>* peaksToJoin);
		void takeNBestPeaks(std::vector<std::shared_ptr<PeakPeakDetector::Peak>>* peaksToJoin, size_t nBest);

}

