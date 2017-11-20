#include "PeakDetector.h"
#include <tbb/tbb.h>
#include <algorithm>


namespace PeakDetector
{
	PeakDetector::PeakDetector(bool paralelism)
	{
		this->paralelism = paralelism;
	}


	PeakDetector::~PeakDetector()
	{
	}

	void PeakDetector::detectPeaks(Common::SegmentDays *segment, int * window_size, std::vector<std::vector<PeakPeakDetector::Peak>> *detectedPeaks)
	{
		size_t daysCount = (*segment).getDays()->size();

		if (this->paralelism)
		{
			(*detectedPeaks) = std::vector<std::vector<PeakPeakDetector::Peak>>(daysCount);
			tbb::parallel_for(size_t(0), daysCount, [&](size_t i)
			{
				std::vector<PeakPeakDetector::Peak> peaks = std::vector<PeakPeakDetector::Peak>();
				this->detectPeakInData(((*segment).getDays()->at(i).getData()), &peaks, window_size);
				(*detectedPeaks)[i] = peaks;
			});


		}
		else 
		{
			(*detectedPeaks) = std::vector<std::vector<PeakPeakDetector::Peak>>(daysCount);
			for (size_t i = 0; i < daysCount; i++)
			{
				std::vector<PeakPeakDetector::Peak> peaks = std::vector<PeakPeakDetector::Peak>();
				this->detectPeakInData(((*segment).getDays()->at(i).getData()), &peaks, window_size);
				(*detectedPeaks)[i] = peaks;
			}
		}


		
	}

	void PeakDetector::detectPeakInData(std::vector<Common::TMeasuredValue *> *data, std::vector<PeakPeakDetector::Peak> *peaks, int * windowSize)
	{
		size_t nBestPeaks = 5;
		std::vector<double> fitnessValues;

		std::vector<PeakPeakDetector::Peak> localPeaks = std::vector<PeakPeakDetector::Peak>();

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

		double threshold = sum/((double)fitnessValues.size());


		for (unsigned int i = 0; i < fitnessValues.size(); i++)
		{
			if (threshold < fitnessValues.at(i))
			{
				localPeaks.push_back(PeakPeakDetector::Peak(i, i + (*windowSize) / 2, fitnessValues.at(i)));
			}
		}

		std::vector<PeakPeakDetector::Peak> joinedPeaks = std::vector<PeakPeakDetector::Peak>();
		if(localPeaks.size()>0){
			PeakPeakDetector::Peak current = localPeaks.at(0);
			for (size_t i = 1; i < localPeaks.size(); i++)
			{
				if (current.endIndex >= localPeaks.at(i).startIndex)
				{
					current.endIndex = localPeaks.at(i).endIndex;
				}
				else 
				{
					joinedPeaks.push_back(current);
					current = localPeaks.at(i);
				}
			}
			joinedPeaks.push_back(current);
		}

		for (size_t i = 0; i < joinedPeaks.size(); i++)
		{
			auto current = joinedPeaks.at(i);
			current.fitness = calculateWindowFitness(data, current.startIndex, current.endIndex);
		}

		std::sort((joinedPeaks).begin(), (joinedPeaks).end(), [](PeakPeakDetector::Peak a, PeakPeakDetector::Peak b) { return a.fitness > b.fitness; });
		size_t peaksLength = (joinedPeaks).size() > nBestPeaks ? nBestPeaks : (joinedPeaks).size();
		(*peaks) = std::vector<PeakPeakDetector::Peak >((joinedPeaks).begin(),(joinedPeaks).begin()+peaksLength);
	}

	
	double PeakDetector::calculateWindowFitness(std::vector<Common::TMeasuredValue *> *data, size_t startIndex, size_t endIndex)
	{
		double fitnessSum = 0;
		for (size_t i = startIndex; i < endIndex; i++) {
			// vezmi dve sousedni hodnooty a zjisti jejich rozdil
			double firstValue = (*data).at(i)->ist;
			double nextValue = (*data).at(i + 1)->ist;
			double difference = nextValue - firstValue;
			//Druha mocnina, aby nebyla zaporna
			fitnessSum += difference * difference;
		}

		return fitnessSum;
	}

}