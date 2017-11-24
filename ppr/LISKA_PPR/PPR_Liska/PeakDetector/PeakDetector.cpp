#include "PeakDetector.h"
#include <tbb/tbb.h>
#include <algorithm>


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
		std::vector<double> fitnessValues;

		std::vector<std::shared_ptr<PeakPeakDetector::Peak>> localPeaks = std::vector<std::shared_ptr<PeakPeakDetector::Peak>>();

		//prochazime postupne vsechny okenka a urcujeme jejich fitness
		for (unsigned int i = 0; i + (*windowSize) < (*data).size(); i++)
		{
			fitnessValues.push_back(calculateWindowFitness(data, i, i + (*windowSize)));
		}

		double sum = 0;
		for (unsigned int i = 0; i < (fitnessValues).size(); i++)
		{
			sum += fitnessValues.at(i);
			//fitnessValues.push_back(calculateWindowFitness(data, i, i + (*windowSize)));
		}

		double threshold = sum / ((double)fitnessValues.size());


		for (unsigned int i = 0; i < fitnessValues.size(); i++)
		{
			if (threshold < fitnessValues.at(i))
			{
				localPeaks.push_back(std::make_unique<PeakPeakDetector::Peak>(PeakPeakDetector::Peak(i, i + (*windowSize) / 2, fitnessValues.at(i))));
			}
		}


		std::vector<std::shared_ptr<PeakPeakDetector::Peak>> joinedPeaks = std::vector<std::shared_ptr<PeakPeakDetector::Peak>>();
		if (localPeaks.size() > 0) {
			std::shared_ptr<PeakPeakDetector::Peak> current = std::shared_ptr<PeakPeakDetector::Peak>(std::move(localPeaks.at(0)));
			for (size_t i = 1; i < localPeaks.size(); i++)
			{
				if ((*current).endIndex >= localPeaks.at(i).get()->startIndex)
				{
					(*current).endIndex = localPeaks.at(i).get()->endIndex;
				}
				else
				{
					joinedPeaks.push_back(current);
					current = std::shared_ptr<PeakPeakDetector::Peak>(std::move(localPeaks.at(i)));
				}
			}
			joinedPeaks.push_back(current);
		}

		for (size_t i = 0; i < joinedPeaks.size(); i++)
		{
			joinedPeaks.at(i).get()->fitness = calculateWindowFitness(data, (joinedPeaks.at(i).get())->startIndex, (joinedPeaks.at(i).get())->endIndex);
		}
		std::map<double, std::shared_ptr<PeakPeakDetector::Peak>> joinedOrderedPeaks = std::map<double, std::shared_ptr<PeakPeakDetector::Peak>>();

		for (size_t i = 0; i < joinedPeaks.size(); i++)
		{
			joinedOrderedPeaks.insert(std::pair<double, std::shared_ptr<PeakPeakDetector::Peak>>(joinedPeaks.at(i).get()->fitness, joinedPeaks.at(i)));
		}

		//Vyber n nejlepsich

		std::map<double, std::shared_ptr<PeakPeakDetector::Peak>>::reverse_iterator rit;
		int counter = 0;
		joinedPeaks = std::vector<std::shared_ptr<PeakPeakDetector::Peak>>();

		for (rit = joinedOrderedPeaks.rbegin(); rit != joinedOrderedPeaks.rend() && (counter<nBestPeaks); ++rit)
		{
			joinedPeaks.push_back(rit->second);
		}

		peaks = std::make_shared<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>(joinedPeaks);
	}

}