#include "PeakDetectorAMP.h"
#include <iostream>
#include <tbb\tbb.h>
#include "Peak.h"
#include "SVGExporter.h"


namespace PeakDetectorAMP
{

	void calcFitnessValues(concurrency::array_view<const double, 1> data, int from, int to, int windowSize, concurrency::array_view<double, 1> fitness) restrict(amp)
	{
		double currentFitness = 0;
		for (int i = from; (i + windowSize) <= to; i++)
		{
			double totalFitness = 0;

			for (int j = i; j < (i + windowSize); j++)
			{
				currentFitness = 0;
				double dataSecond = data[j];
				double dataFirst = data[j + 1];
				currentFitness = dataFirst - dataSecond;
				currentFitness = currentFitness*currentFitness;
				totalFitness += currentFitness;
			}

			fitness[i] = totalFitness;
		}

	}

	void calcFitnesValuesFromTo(concurrency::array_view<const double, 1> data, concurrency::array_view<int, 1> peakStartIndexes, concurrency::array_view<int, 1> peakEndIndexes, int from, int to, int windowSize, concurrency::array_view<double, 1> fitness) restrict(amp)
	{
		int currentFrom = 0;
		int currentTo = 0;
		double currentFitness = 0;

		for (int i = from; i < to; i++)
		{
			currentFrom = peakStartIndexes[i];
			currentTo = peakEndIndexes[i];
			if (currentTo == 0)
			{
				break;
			}
			double totalFitness = 0;

			for (int j = currentFrom; (j) < currentTo; j++)
			{
				currentFitness = 0;
				double dataSecond = data[j];
				double dataFirst = data[j + 1];
				currentFitness = dataFirst - dataSecond;
				currentFitness = currentFitness*currentFitness;
				totalFitness += currentFitness;
			}

			fitness[i] = totalFitness;

		}
	}



	//void detectPeaks(concurrency::array_view<const double, 1> data, int from, int to, int windowSize, concurrency::array_view<double, 1> peakValues) restrict(amp)
	//{
	//	for (int i = from; i < to; i++)
	//	{
	//		peakValues[i] = data[i] * data[i];
	//	}
	//}

	void insertionSort(concurrency::array_view<double, 1> fitness, int from, int to) restrict(amp) {

		for (int i = from; i < to; i++) {
			int j = i + 1;
			int tmp = fitness[j];
			while (j > from && tmp > fitness[j - 1]) {
				fitness[j] = fitness[j - 1];
				j--;
			}
			fitness[j] = tmp;
		}


	}

	double runOnGraphics(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data)
	{

		int windowSize = params->getWindowSize();
		//Prevest data na data pro vypocet
		std::vector<int> dayCount;
		std::vector<int> segmentDaysStartIndexes;
		std::vector<int> segmentDaysEndIndexes;
		std::vector<double> istValues;


		int indexCounter = 0;
		for (size_t i = 0; i < (*data).size(); i++)
		{
			auto currentSegment = (*data).at(i).get();
			if (currentSegment->getSegmentDays() != nullptr)
			{
				//segmentStartIndexes.push_back(indexCounter);
				auto currentDays = currentSegment->getSegmentDays()->getDays();
				dayCount.push_back(currentDays->size());
				for (size_t j = 0; j < currentDays->size(); j++)
				{
					segmentDaysStartIndexes.push_back(indexCounter);
					auto currentData = currentDays->at(j).get()->getData();
					for (size_t k = 0; k < currentData->size(); k++)
					{
						auto currentItem = currentData->at(k).get();
						istValues.push_back((*currentItem).ist);
						indexCounter++;
					}
					segmentDaysEndIndexes.push_back(indexCounter - 1);
				}
			}
		}

		std::vector<double> fitnessValues = std::vector<double>(indexCounter);
		std::vector<int> peaksStartIndexes = std::vector<int>(indexCounter);
		std::vector<int> peaksEndIndexes = std::vector<int>(indexCounter);
		std::vector<int> joinedPeaksStartIndexes = std::vector<int>(indexCounter);
		std::vector<int> joinedPeaksEndIndexes = std::vector<int>(indexCounter);
		std::vector<double> peakFintessValues = std::vector<double>(indexCounter);

		concurrency::array_view<int, 1> segmentDaysStartIndexesView((int)segmentDaysStartIndexes.size(), segmentDaysStartIndexes.data());
		concurrency::array_view<int, 1> segmentDaysEndIndexesView((int)segmentDaysEndIndexes.size(), segmentDaysEndIndexes.data());
		concurrency::array_view<const double, 1> istValuesView((int)istValues.size(), istValues.data());
		concurrency::array_view<double, 1> resultView(indexCounter, fitnessValues.data());
		concurrency::array_view<int, 1> peaksStartIndexesView((int)peaksStartIndexes.size(), peaksStartIndexes.data());
		concurrency::array_view<int, 1> peaksEndIndexesView((int)peaksEndIndexes.size(), peaksEndIndexes.data());
		concurrency::array_view<int, 1> joinedPeaksStartIndexesView((int)joinedPeaksStartIndexes.size(), joinedPeaksStartIndexes.data());
		concurrency::array_view<int, 1> joinedPeaksEndIndexesView((int)joinedPeaksEndIndexes.size(), joinedPeaksEndIndexes.data());
		concurrency::array_view<double, 1> peakFintessValuesView(indexCounter, peakFintessValues.data());


		tbb::tick_count before = tbb::tick_count::now();

		Concurrency::parallel_for_each(segmentDaysStartIndexesView.extent, [=](concurrency::index<1> idx) restrict(amp)
		{
			int dayFrom = segmentDaysStartIndexesView[idx];
			int dayTo = segmentDaysEndIndexesView[idx];
			//Mame spocitana okna pro vsechny
			calcFitnessValues(istValuesView, dayFrom, dayTo, windowSize, resultView);
			//V ramci dnu mame spocitanou prumernou hodnotu
			//Ted musim vytvorit peaky - rovnou je i vyfiltruju
			initPeaks(resultView, calcDayAverageFitness(dayFrom, dayTo, resultView), windowSize, dayFrom, dayTo, peaksStartIndexesView, peaksEndIndexesView);
			//Spojit je dohromady
			joinPeaks(dayFrom, dayTo, peaksStartIndexesView, peaksEndIndexesView, joinedPeaksStartIndexesView, joinedPeaksEndIndexesView);
			////Vypocitat pro peaky ohodnoceni
			calcFitnesValuesFromTo(istValuesView, joinedPeaksStartIndexesView, joinedPeaksEndIndexesView, dayFrom, dayTo, windowSize, peakFintessValuesView);
			//Seradit
			insertionSort(peakFintessValuesView, dayFrom, dayTo);
		});
		resultView.synchronize();
		peaksStartIndexesView.synchronize();
		peaksEndIndexesView.synchronize();
		joinedPeaksStartIndexesView.synchronize();
		joinedPeaksEndIndexesView.synchronize();
		peakFintessValuesView.synchronize();
		
		//Segment
		//Den
		//Peaky
		std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>> segmentSPeaks = std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>();
		int maxDayPeaks = 5;
		int currentCounter = 0;
		int currentCountDays = 0;
		int processedDayCount = 0;
		int currentFrom = 0;
		int currentTo = 0;
		int currentSegmentCounter = 0;
		for (int i = 0; i < dayCount.size(); i++)
		{
			currentCountDays = dayCount.at(i);
			//Pro kazdy den ziskat peaky
			for (int j = 0; j < currentCountDays; j++)
			{
				std::vector<std::shared_ptr<PeakPeakDetector::Peak>> currentDayPeaks = std::vector<std::shared_ptr<PeakPeakDetector::Peak>>();

				currentCounter = 0;
				currentFrom = segmentDaysStartIndexes.at(processedDayCount);
				currentTo = segmentDaysEndIndexes.at(processedDayCount);
				for (int k = currentFrom; (k < currentTo) && (joinedPeaksEndIndexes.at(k) != 0) && (currentCounter <= maxDayPeaks); k++)
				{
					currentDayPeaks.push_back(std::make_shared<PeakPeakDetector::Peak>(joinedPeaksStartIndexes.at(k) - currentFrom, joinedPeaksEndIndexes.at(k) - currentFrom, peakFintessValues.at(k)));
					currentCounter++;
				}
				segmentSPeaks.push_back(currentDayPeaks);
				processedDayCount++;
			}
			currentSegmentCounter++;

		}

		tbb::tick_count after = tbb::tick_count::now();
		int currentDay = 0;
		int currenDayCounter = 0;
		for (int i = 0; i < data.get()->size(); i++)
		{
			if (data.get()->at(i).get()->getSegmentDays() != nullptr)
			{
				auto currentPeaks = std::make_shared<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>(segmentSPeaks.begin() + currentDay, segmentSPeaks.begin() + currentDay + dayCount.at(currenDayCounter));
				currentDay += dayCount.at(currenDayCounter);
				//MySVG::exportToSvg((*params).getExportPath(), data.get()->at(i).get(), currentPeaks, true);
				currenDayCounter++;
			}
		}
		return (after - before).seconds();

	}

	void joinPeaks(int from, int to, concurrency::array_view<int, 1> peakStartIndexes, concurrency::array_view<int, 1> peakEndIndexes, concurrency::array_view<int, 1> resultPeakStartIndexes, concurrency::array_view<int, 1> resultPeakEndIndexes) restrict(amp)
	{

		int currentStartIndex = peakStartIndexes[from];
		int currentEndIndex = peakEndIndexes[from];
		int counter = from;

		for (int i = from + 1; (i < to); i++)
		{
			if (currentEndIndex >= peakStartIndexes[i])
			{
				if (peakStartIndexes[i] != 0)
				{
					currentEndIndex = peakEndIndexes[i];
				}
				else
				{
					break;
				}
			}
			else
			{
				resultPeakStartIndexes[counter] = currentStartIndex;
				resultPeakEndIndexes[counter] = currentEndIndex;
				if (peakStartIndexes[i] != 0)
				{
					currentStartIndex = peakStartIndexes[i];
					currentEndIndex = peakEndIndexes[i];
				}
				else
				{
					break;
				}
				counter += 1;
			}
		}

		resultPeakStartIndexes[counter] = currentStartIndex;
		resultPeakEndIndexes[counter] = currentEndIndex;
	}


	void initPeaks(concurrency::array_view<double, 1> data, double average, int windowsSize, int from, int to, concurrency::array_view<int, 1> peakStartIndexes, concurrency::array_view<int, 1> peakEndIndexes)restrict(amp)
	{
		int selector = from;
		if (average != 0)
		{
			for (int i = from; i < (to); i++)
			{
				if (data[i] > average)
				{
					peakStartIndexes[selector] = i;
					peakEndIndexes[selector] = i + (windowsSize);
					selector += 1;
				}
			}
		}
	}



	double calcDayAverageFitness(int from, int to, concurrency::array_view<double, 1> fitness) restrict(amp)
	{
		//int nonNullValues = 0;
		double sum = 0;
		int nonNullValues = 0;
		double currentFitness = 0;
		for (int i = from; i < to; i++)
		{
			currentFitness = fitness[i];
			if (currentFitness > 0)
			{
				nonNullValues++;
			}
			sum += currentFitness;
		}
		if (nonNullValues == 0)
		{
			return 0;
		}

		return (sum / (double(nonNullValues)));
	}



}
