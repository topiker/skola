#include "PeakDetectorAMP.h"
#include <iostream>


namespace PeakDetectorAMP
{
	void detectPeaks(int x) restrict(amp)
	{

	}

	void v20() 
	{
		const int size = 5;
		int aCPP[] = { 1, 2, 3, 4, 5 };
		int bCPP[] = { 6, 7, 8, 9, 10 };
		int sumCPP[size];

		// Create C++ AMP objects.  
		concurrency::array_view<const int, 1> a(size, aCPP);
		concurrency::array_view<const int, 1> b(size, bCPP);
		concurrency::array_view<int, 1> sum(size, sumCPP);
		sum.discard_data();

		concurrency::parallel_for_each(
			// Define the compute domain, which is the set of threads that are created.  
			sum.extent,
			// Define the code to run on each thread on the accelerator.  
			[=](concurrency::index<1> idx) restrict(amp)
		{
			sum[idx] = a[idx] + b[idx];
		}
		);

		// Print the results. The expected output is "7, 9, 11, 13, 15".  
		for (int i = 0; i < size; i++) {
			std::cout << sum[i] << "\n";
		}
	}

	void runOnGraphics(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data)
	{
		

		////Prevest data na data pro vypocet
		//std::vector<int> segmentStartIndexes;
		//std::vector<int> segmentEndIndexes;
		//std::vector<int> segmentDaysStartIndexes;
		//std::vector<int> segmentDaysEndIndexes;
		//std::vector<double> istValues;

		//int indexCounter = 0;
		//for (size_t i = 0; i < (*data).size(); i++)
		//{
		//	auto currentSegment = (*data).at(i).get();
		//	if (currentSegment->getSegmentDays() != nullptr)
		//	{
		//		segmentStartIndexes.push_back(indexCounter);
		//		auto currentDays = currentSegment->getSegmentDays()->getDays();
		//		for (size_t j = 0; j < currentDays->size(); j++)
		//		{
		//			segmentDaysStartIndexes.push_back(indexCounter);
		//			auto currentData = currentDays->at(j).get()->getData();
		//			for (size_t k = 0; k < currentData->size(); k++)
		//			{
		//				auto currentItem = currentData->at(k).get();
		//				istValues.push_back((*currentItem).ist);
		//				indexCounter++;
		//			}
		//			segmentDaysEndIndexes.push_back(indexCounter - 1);
		//		}
		//		segmentEndIndexes.push_back(indexCounter - 1);
		//	}
		//}

		//concurrency::array_view<int, 1> segmentStartIndexesView((int)segmentStartIndexes.size(), segmentStartIndexes.data());
		//concurrency::array_view<int, 1> segmentEndIndexesView((int)segmentEndIndexes.size(), segmentEndIndexes.data());
		//concurrency::array_view<int, 1> segmentDaysStartIndexesView((int)segmentDaysStartIndexes.size(), segmentDaysStartIndexes.data());
		//concurrency::array_view<int, 1> segmentDaysEndIndexesView((int)segmentDaysEndIndexes.size(), segmentDaysEndIndexes.data());
		//concurrency::array_view<const double, 1> istValuesView((int)istValues.size(), istValues.data());

		//Concurrency::parallel_for_each(segmentStartIndexesView.extent, [=](concurrency::index<1> idx) restrict(amp)
		//{
		//	PeakDetectorAMP::detectPeaks(segmentStartIndexesView[idx]);
		//});


	}


}
