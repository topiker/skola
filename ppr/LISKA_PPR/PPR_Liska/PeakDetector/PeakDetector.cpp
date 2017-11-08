#include "PeakDetector.h"


namespace PeakDetector
{
	PeakDetector::PeakDetector()
	{
	}


	PeakDetector::~PeakDetector()
	{
	}

	void PeakDetector::detectPeaks(std::vector<TMeasuredValue *> *data, int *window_size, std::vector<Peak> *detectedPeaks)
	{
		std::vector<double> fitnessValues;

		//prochazime postupne vsechny okenka a urcujeme jejich fitness
		for (unsigned int i = 0; i + (*window_size) < (*data).size(); i++) {
			fitnessValues.push_back(calculateWindowFitness(data, i, i + (*window_size)));
		}


		for (unsigned int i = 0; i < fitnessValues.size(); i++) {
			//najdeme index okenka s maximalni hodnotou v intervalu  i + window_size/2
			unsigned int maxValueIndex = intervalMaxValueIndex(i, i + (*window_size) / 2, &fitnessValues);

			//hledej index nejlepsiho okenka, dokud se dari najit ve vzdalenosti window_size/2 okenko s vyssi fitness.
			while (maxValueIndex != i && maxValueIndex < fitnessValues.size()) {
				// pokud maxValueIndex == i -> ve vzdalenosti window_size/2 neexistuje okenko s lepsi fitness, cyklus konci

				// zmenime aktualni pozici na nove nalezene nejlepsi okenko
				i = maxValueIndex;

				// zkusime se podivat jestli ve vzdalenosti window_size/2 neexistuje jeste lepsi okenko nez aktualne nalezene1
				maxValueIndex = intervalMaxValueIndex(i, i + (*window_size) / 2, &fitnessValues);
			}
			// po skonceni cylku vime ze v okoli aktualne nalezeneho okenka neexistuje zadne lepsi


			(*detectedPeaks).push_back(Peak::Peak(i, i + (*window_size) / 2));

			//posuneme index o velikost pulku okenka doprava a zacneme hledat nove okenko
			i += (*window_size) / 2;

			//TODO:Vybrat pouze ty nejlepsi
			// -> tahle metoda muze najit velike mnozstvi okenek, jeste to bude chtit vyfiltrovat treba 5 nejlepsich
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

	void PeakDetector::smooth_null_values(std::vector<TMeasuredValue *> *data)
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

	double PeakDetector::calculateWindowFitness(std::vector<TMeasuredValue *> *data, int startIndex, int endIndex)
	{
		double fitnessSum = 0;
		for (int i = startIndex; i < endIndex; i++) {
			// vezmi dve sousedni hodnooty a zjisti jejich rozdil
			double firstValue = (*data).at(i)->ist;
			double nextValue = (*data).at(i + 1)->ist;
			double difference = nextValue - firstValue;
			// udelej druhou mocninu rozdilu a pricti k fitness
			fitnessSum += difference * difference;
		}

		return fitnessSum;
	}

}