#include "PeakDetector.h"
#include <tbb/tbb.h>
#include <algorithm>
#include <map>


namespace PeakDetector
{

	void detectPeaks(Common::SegmentDays*const segment, int * windowSize, std::shared_ptr< std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& detectedPeaks, bool paralelismPerDay)
	{
		size_t daysCount = (*segment).getDaysSize();
		if (paralelismPerDay)
		{
			detectedPeaks = std::make_unique<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>(daysCount);
			std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>> result = std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>(daysCount);
			tbb::parallel_for(size_t(0), daysCount, [&](size_t i)
			{
				std::shared_ptr<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>> peaks = std::shared_ptr<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>();
				detectPeakInData(segment->getDays()->at(i).get()->getData(), peaks, windowSize);
				result[i].insert(std::move(result[i].begin()), peaks.get()->begin(), peaks.get()->end());
			});
		}
		else
		{
			detectedPeaks = std::make_unique<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>(daysCount);
			std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>> result = std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>(daysCount);
			for (size_t i = 0; i < daysCount; i++)
			{
				std::shared_ptr<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>> peaks = std::shared_ptr<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>();
				detectPeakInData(segment->getDays()->at(i).get()->getData(), peaks, windowSize);
				result[i].insert(std::move(result[i].begin()), peaks.get()->begin(), peaks.get()->end());
			}
			(detectedPeaks) = std::make_shared<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>(result.begin(), result.end());
		}


	}

	double calculateWindowFitness(std::vector<std::unique_ptr<Common::TMeasuredValue>>* const data, size_t startIndex, size_t endIndex)
	{
			double fitnessSum = 0;
			for (size_t i = startIndex; i < endIndex; i++) {
				// vezmi dve sousedni hodnooty a zjisti jejich rozdil
				double difference = (*data).at(i + 1)->ist - (*data).at(i)->ist;
				//Druha mocnina, aby nebyla zaporna
				fitnessSum += difference * difference;
			}

			return fitnessSum;
	}

	void detectPeakInData(std::vector<std::unique_ptr<Common::TMeasuredValue>>* const data, std::shared_ptr<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>& peaks, int * windowSize)
	{
		size_t nBestPeaks = 5;
		std::vector<double> fitnessValues = getFitnessOfAllWindows(data,windowSize);
		double threshold = getAvgFitnessValue(&fitnessValues);
		std::vector<std::shared_ptr<PeakPeakDetector::Peak>> localPeaks = std::vector<std::shared_ptr<PeakPeakDetector::Peak>>();
		//Filtrovani tech oken, ktera jsou lepsi nez prumer
		for (unsigned int i = 0; i < fitnessValues.size(); i++)
		{
			if (threshold < fitnessValues.at(i))
			{
				localPeaks.push_back(std::make_unique<PeakPeakDetector::Peak>(PeakPeakDetector::Peak(i, i + (*windowSize), fitnessValues.at(i))));
			}
		}
		//Spojim prekryvajici se okna
		auto joinedPeaks = joinOverlayedPeaks(&localPeaks);
		//Spocitam nove spojenym oknum fitness
		recalculatePeaksFitness(data, &joinedPeaks);
		takeNBestPeaks(&joinedPeaks, nBestPeaks);
		peaks = std::make_shared<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>(joinedPeaks);
	}

	void takeNBestPeaks(std::vector<std::shared_ptr<PeakPeakDetector::Peak>>* peaks, size_t nBest)
	{
	
		std::map<double, std::shared_ptr<PeakPeakDetector::Peak>> joinedOrderedPeaks = std::map<double, std::shared_ptr<PeakPeakDetector::Peak>>();
		for (size_t i = 0; i < (*peaks).size(); i++)
		{
			joinedOrderedPeaks.insert(std::pair<double, std::shared_ptr<PeakPeakDetector::Peak>>((*peaks).at(i).get()->fitness, (*peaks).at(i)));
		}

		//Vyber n nejlepsich
		std::map<double, std::shared_ptr<PeakPeakDetector::Peak>>::reverse_iterator rit;
		int counter = 0;
		(*peaks) = std::vector<std::shared_ptr<PeakPeakDetector::Peak>>();

		for (rit = joinedOrderedPeaks.rbegin(); rit != joinedOrderedPeaks.rend() && (counter<nBest); ++rit)
		{
			(*peaks).push_back(rit->second);
		}
	}


	std::vector<double> getFitnessOfAllWindows(std::vector<std::unique_ptr<Common::TMeasuredValue>>* const data, int * windowSize)
	{
		std::vector<double> fitnessValues = std::vector<double>();
		//prochazime postupne vsechny okenka a urcujeme jejich fitness
		for (unsigned int i = 0; (i + (*windowSize)) < (*data).size(); i++)
		{
			fitnessValues.push_back(calculateWindowFitness(data, i, (i + (*windowSize))));
		}

		return fitnessValues;
	}

	double getAvgFitnessValue(std::vector<double>*avg)
	{
		double sum = 0;
		for (unsigned int i = 0; i < ((*avg)).size(); i++)
		{
			sum += (*avg).at(i);
		}

		return (sum) / ((double)(*avg).size());
	}

	std::vector<std::shared_ptr<PeakPeakDetector::Peak>> joinOverlayedPeaks(std::vector<std::shared_ptr<PeakPeakDetector::Peak>>* peaksToJoin)
	{
		std::vector<std::shared_ptr<PeakPeakDetector::Peak>> joinedPeaks = std::vector<std::shared_ptr<PeakPeakDetector::Peak>>();
		if ((*peaksToJoin).size() > 0)
		{
			std::shared_ptr<PeakPeakDetector::Peak> current = std::shared_ptr<PeakPeakDetector::Peak>(std::move((*peaksToJoin).at(0)));
			for (size_t i = 1; i < (*peaksToJoin).size(); i++)
			{
				if ((*current).endIndex >= (*peaksToJoin).at(i).get()->startIndex)
				{
					(*current).endIndex = (*peaksToJoin).at(i).get()->endIndex;
				}
				else
				{
					joinedPeaks.push_back(current);
					current = std::shared_ptr<PeakPeakDetector::Peak>(std::move((*peaksToJoin).at(i)));
				}
			}
			joinedPeaks.push_back(current);
		}
		return joinedPeaks;
	}

	void recalculatePeaksFitness(std::vector<std::unique_ptr<Common::TMeasuredValue>>*const data, std::vector<std::shared_ptr<PeakPeakDetector::Peak>>* peaks)
	{
		for (size_t i = 0; i < (*peaks).size(); i++)
		{
			(*peaks).at(i).get()->fitness = calculateWindowFitness(data, ((*peaks).at(i).get())->startIndex, ((*peaks).at(i).get())->endIndex);
		}
	}


}