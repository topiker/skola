#pragma once
#include <amp.h>
#include <memory>

#include "InputParser.h"
#include"Segment.h"


namespace PeakDetectorAMP
{

	void calcFitnessValues(concurrency::array_view<const double, 1> data, int from, int to, int windowSize, concurrency::array_view<double, 1> fitness) restrict(amp);
	void calcFitnesValuesFromTo(concurrency::array_view<const double, 1> data, concurrency::array_view<int, 1> peakStartIndexes, concurrency::array_view<int, 1> peakEndIndexes, int from, int to, int windowsize, concurrency::array_view<double, 1> fitness) restrict(amp);
	double calcDayAverageFitness(int from, int to, concurrency::array_view<double, 1> fitness) restrict(amp);
	void joinPeaks(int from, int to, concurrency::array_view<int, 1> peakStartIndexes, concurrency::array_view<int, 1> peakEndIndexes, concurrency::array_view<int, 1> resultPeakStartIndexes, concurrency::array_view<int, 1> resultPeakEndIndexes) restrict(amp);
	void initPeaks(concurrency::array_view<double, 1> data, double average, int windowsSize, int from, int to, concurrency::array_view<int, 1> peakStartIndexes, concurrency::array_view<int, 1> peakEndIndexes) restrict(amp);
	double runOnGraphics(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data);


}

