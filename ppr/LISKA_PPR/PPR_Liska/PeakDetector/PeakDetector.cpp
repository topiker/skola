#include "PeakDetector.h"
#include <algorithm>


namespace PeakDetector
{
	PeakDetector::PeakDetector()
	{
	}


	PeakDetector::~PeakDetector()
	{
	}

	void PeakDetector::detectPeaks(std::vector<Common::SegmentDay> *days, int * window_size, std::vector<std::vector<PeakPeakDetector::Peak>> *detectedPeaks)
	{
		(*detectedPeaks) = std::vector<std::vector<PeakPeakDetector::Peak>>();
		for (size_t i = 0; i < (*days).size(); i++)
		{
			std::vector<PeakPeakDetector::Peak> peaks = std::vector<PeakPeakDetector::Peak>();
			this->detectPeakInDay(&((*days).at(i).data), &peaks, window_size);
			(detectedPeaks)->push_back(peaks);
		}
	}

	void PeakDetector::detectPeakInDay(std::vector<Common::TMeasuredValue *> *data, std::vector<PeakPeakDetector::Peak> *peaks, int * windowSize)
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

	void PeakDetector::smooth_null_values(std::vector<Common::TMeasuredValue *> *data)
	{
		double lastNonNullValue = 0;
		for (unsigned int i = 0; i < (*data).size(); i++) {
			if ((*data).at(i)->ist == NULL) {
				//nasli jsme null
				if (lastNonNullValue != 0) {
					// lastNonNullValue je nenulova, to znamena, ze uz jsme drive nasli validni hodnotu a tak ji nastavime misto nullu.
					(*data).at(i)->ist = lastNonNullValue;
				}
			}
			else {
				// aktualni hodnota neni null
				if (lastNonNullValue == 0 && i != 0) {
					// v predchozich pruchodech jsme dosud nenasli zadnou validni hodnotu (lastNonNullValue == 0) a  i != 0 -> to implikuje ze vsechny doposud od zacatku prectene hodnoty byly null
					// v cyklu pujdeme zpet az na zacatek a vsem hodnotam nastavime aktualne nalezenou validni hodnotu 
					for (int j = i; j >= 0; j--) {
						(*data).at(j)->ist = (*data).at(i)->ist;
					}
				}
				//aktualizace posledni validni nenulove hodnoty
				lastNonNullValue = (*data).at(i)->ist;
			}
		}
	}

	void PeakDetector::moving_average(std::vector<Common::TMeasuredValue *> *data, int *windowSize)
	{
		if ((*data).size() > (*windowSize))
		{
			double runningTotal = 0;
			Common::TMeasuredValue * prev = nullptr;
			Common::TMeasuredValue * current = nullptr;
			for (int i = 0; i < (*windowSize); i++)
			{
				runningTotal += (*data).at(i)->ist;
			}
			//Spocitat pocatecni mean
			for (int i = 0; i < (*data).size(); i++)
			{
				current = (*data).at(i);
				if (prev != nullptr)
				{
					runningTotal -= prev->ist;   // subtract
					runningTotal += current->ist;  // add
					current->smoothedValue = runningTotal / (double)(*windowSize);
				}
				else
				{
					current->smoothedValue = runningTotal/(double)(*windowSize);
				}
				prev = current;
			}
		}
		//TODO:Osetrit
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