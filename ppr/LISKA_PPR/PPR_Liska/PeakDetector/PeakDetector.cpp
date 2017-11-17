#include "PeakDetector.h"


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
		std::vector<double> fitnessValues;

		//prochazime postupne vsechny okenka a urcujeme jejich fitness
		for (unsigned int i = 0; i + (*windowSize) < (*data).size(); i++)
		{
			fitnessValues.push_back(calculateWindowFitness(data, i, i + (*windowSize)));
		}

		for (unsigned int i = 0; i < fitnessValues.size(); i++)
		{
			//najdeme index okenka s maximalni hodnotou v intervalu  i + windowSize/2
			unsigned int maxValueIndex = intervalMaxValueIndex(i, i + (*windowSize) / 2, &fitnessValues);

			//hledej index nejlepsiho okenka, dokud se dari najit ve vzdalenosti windowSize/2 okenko s vyssi fitness.
			while (maxValueIndex != i && maxValueIndex < fitnessValues.size()) {
				// pokud maxValueIndex == i -> ve vzdalenosti windowSize/2 neexistuje okenko s lepsi fitness, cyklus konci

				// zmenime aktualni pozici na nove nalezene nejlepsi okenko
				i = maxValueIndex;

				// zkusime se podivat jestli ve vzdalenosti windowSize/2 neexistuje jeste lepsi okenko nez aktualne nalezene1
				maxValueIndex = intervalMaxValueIndex(i, i + (*windowSize) / 2, &fitnessValues);
			}
			// po skonceni cylku vime ze v okoli aktualne nalezeneho okenka neexistuje zadne lepsi

			(*peaks).push_back(PeakPeakDetector::Peak(i - (*windowSize) / 2, i + (*windowSize) / 2, 5));

			i += (*windowSize);
		}


	}

	//projdeme fitness hodnoty okenek od startIndex do endIndex a vratime index nejlepsiho okenka
	int PeakDetector::intervalMaxValueIndex(unsigned int startIndex, unsigned int endIndex, std::vector<double> *fitnessValues) {
		int maxValueIndex = startIndex;
		double maxValue = (*fitnessValues).at(startIndex);
		for (unsigned int i = startIndex; i < endIndex && i < (*fitnessValues).size(); i++) {
			if ((*fitnessValues).at(i) > maxValue) {
				maxValueIndex = i;
				maxValue = (*fitnessValues).at(i);
			}
		}
		return maxValueIndex;
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
			//Spocitat pocatecni mean
			for (int i = 0; i < (*data).size(); i++)
			{
				Common::TMeasuredValue * current = (*data).at(i);
				if (prev != nullptr)
				{
					runningTotal -= prev->ist;   // subtract
					runningTotal += current->ist;  // add
					current->smoothedValue = runningTotal / (double)(*windowSize);
				}
				else
				{
					runningTotal = current->ist;
					current->smoothedValue = runningTotal;
				}
				prev = current;
			}
		}
		int x = 5;
	}

	double PeakDetector::calculateWindowFitness(std::vector<Common::TMeasuredValue *> *data, int startIndex, int endIndex)
	{
		double fitnessSum = 0;
		for (int i = startIndex; i < endIndex; i++) {
			// vezmi dve sousedni hodnooty a zjisti jejich rozdil
			double firstValue = (*data).at(i)->smoothedValue;// ((*data).at(i)->ist - (*data).at(i)->smoothedValue)*((*data).at(i)->ist - (*data).at(i)->smoothedValue);
			double nextValue = (*data).at(i + 1)->smoothedValue;//;((*data).at(i+1)->ist - (*data).at(i+1)->smoothedValue)*((*data).at(i+1)->ist - (*data).at(i+1)->smoothedValue);
			double difference = nextValue - firstValue;
			// udelej druhou mocninu rozdilu a pricti k fitness
			fitnessSum += difference * difference;
		}

		return fitnessSum;
	}

}