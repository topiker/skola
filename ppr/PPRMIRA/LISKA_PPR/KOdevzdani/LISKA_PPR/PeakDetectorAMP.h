#pragma once
#include <amp.h>
#include <memory>

#include "InputParser.h"
#include"Segment.h"
#include "Peak.h"


namespace PeakDetectorAMP
{
	/// <summary>
	/// Spocita ve vstupnich datech vsechny fitness hodnoty okenek
	/// </summary>
	void calcFitnessValues(concurrency::array_view<const double, 1> data, int from, int to, int windowSize, concurrency::array_view<double, 1> fitness) restrict(amp);
	/// <summary>
	/// Spocita fitnes hodnotu v datech od do
	/// </summary>
	void calcFitnesValuesFromTo(concurrency::array_view<const double, 1> data, concurrency::array_view<int, 1> peakStartIndexes, concurrency::array_view<int, 1> peakEndIndexes, int from, int to, concurrency::array_view<double, 1> fitness) restrict(amp);
	/// <summary>
	/// Spocita prumernou hodnotu fitness v intervalu od do. Nezahrnuje 0 hodnoty
	/// </summary>
	double calcDayAverageFitness(int from, int to, concurrency::array_view<double, 1> fitness) restrict(amp);
	/// <summary>
	/// Spojuje prekryvajici se okenka dohromady v intervalu od do, spojena okenka jsou pak v resultPeakStartIndexes a resultPeakEndIndexes
	/// </summary>
	void joinPeaks(int from, int to, concurrency::array_view<int, 1> peakStartIndexes, concurrency::array_view<int, 1> peakEndIndexes, concurrency::array_view<int, 1> resultPeakStartIndexes, concurrency::array_view<int, 1> resultPeakEndIndexes) restrict(amp);
	/// <summary>
	/// Procedura slouzi pro inicializaci okenek, rovnou i filturje ty okenka, jejich fitnes funkce je mensi nez hodnota average
	/// </summary>
	void initPeaks(concurrency::array_view<double, 1> data, double average, int windowsSize, int from, int to, concurrency::array_view<int, 1> peakStartIndexes, concurrency::array_view<int, 1> peakEndIndexes) restrict(amp);
	/// <summary>
	/// Ze vstupnich dat vypocita vyznamna okenka -> allPeaks
	/// </summary>
	double runOnGraphics(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data, std::vector<std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>> &allPeaks);


}

