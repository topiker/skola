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
		void detectPeaks(const std::shared_ptr<Common::SegmentDays>& days, int * windowSize, std::shared_ptr< std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& detectedPeaks);

	private:
		//double calculateWindowFitness(std::vector<Common::TMeasuredValue *> *data, size_t startIndex, size_t endIndex);
		double calculateWindowFitness(const std::shared_ptr<std::vector<std::shared_ptr<Common::TMeasuredValue>>>& data, size_t startIndex, size_t endIndex);

		void detectPeakInData(const std::shared_ptr<std::vector<std::shared_ptr<Common::TMeasuredValue>>>& data, std::shared_ptr<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>& peaks, int * windowSize);
		bool paralelism;
	};

}

