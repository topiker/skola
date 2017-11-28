#pragma once


#include "TMeasuredValue.h"
#include <vector>
#include "Peak.h"
#include "SegmentDays.h"
#include <amp.h>

namespace  PeakDetector
{
	/// <summary>
	/// Detekuje peaky v datech; paralelismPerDay odlisuje vypocet, zda se ma paraelelizovat vypocet i v ramci dne
	/// </summary>
	void detectPeaks(Common::SegmentDays*const days, int * windowSize, std::shared_ptr< std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& detectedPeaks, bool paralelismPerDay);
	/// <summary>
	/// Spocita fitnes hodnotu v datech od do
	/// </summary>
	double calculateWindowFitness(std::vector<std::unique_ptr<Common::TMeasuredValue>>* const data, size_t startIndex, size_t endIndex);
	/// <summary>
	/// Slouzi pro detekci peaku uz v konkretnich datech, vraci nalezena okna. 
	/// </summary>
	void detectPeakInData(std::vector<std::unique_ptr<Common::TMeasuredValue>>*const data, std::shared_ptr<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>& peaks, int * windowSize);
	/// <summary>
	/// Obalujici funkce pro vypocet fintess hodnoty v datech. Kouskuje je po castech
	/// </summary>
	std::vector<double> getFitnessOfAllWindows(std::vector<std::unique_ptr<Common::TMeasuredValue>>* const data, int * windowSize);
	/// <summary>
	/// Vypocita prumernou hodnotu ze vstupnich dat
	/// </summary>
	double getAvgFitnessValue(std::vector<double>*avg);
	/// <summary>
	/// Spojuje okenka, ktera se prekryvaji
	/// </summary>
	std::vector<std::shared_ptr<PeakPeakDetector::Peak>> joinOverlayedPeaks(std::vector<std::shared_ptr<PeakPeakDetector::Peak>>* peaksToJoin);
	/// <summary>
	/// Spocte fitness hodnoty okenkum
	/// </summary>
	void recalculatePeaksFitness(std::vector<std::unique_ptr<Common::TMeasuredValue>>*const data, std::vector<std::shared_ptr<PeakPeakDetector::Peak>>* peaksToJoin);
	/// <summary>
	/// Vezme n nejlepsich okenek
	/// </summary>
	void takeNBestPeaks(std::vector<std::shared_ptr<PeakPeakDetector::Peak>>* peaksToJoin, size_t nBest);

}

