#pragma once
#include <amp.h>
#include <memory>

#include "InputParser.h"
#include"Segment.h"


namespace PeakDetectorAMP
{
	//data, index segmentu od do, jeste propasovat indexy dnu, velikost okna
	void detectPeaks(int x)restrict(amp);

	void v20();

	void runOnGraphics(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data);

	//void detectPeaks(Common::SegmentDays*const days, int * windowSize, std::shared_ptr< std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& detectedPeaks, bool paralelismPerDay);

	//void calculateAllWindowFitness(const concurrency::array_view<const double, 1> data, concurrency::array_view<double, 1> result, unsigned int windowsSize, unsigned int dataLength) restrict(amp);
	//double calculateFitness(const concurrency::array_view<const double, 1> data, unsigned int startIndex, unsigned int endIndex)restrict(amp);

}

