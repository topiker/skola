#pragma once

#ifdef  PEAKDETECTORDLL_EXPORTS 
/*Enabled as "export" while compiling the dll project*/
#define DLLEXPORT __declspec(dllexport)  
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define DLLEXPORT __declspec(dllimport)  
#endif

#include "TMeasuredValue.h"
#include <vector>
#include "Peak.h"
#include "SegmentDay.h"

namespace  PeakDetector
{

	class DLLEXPORT PeakDetector
	{
	public:
		PeakDetector();
		~PeakDetector();
		void detectPeaks(std::vector<Common::SegmentDay> *days, int * windowSize, std::vector<std::vector<PeakPeakDetector::Peak>> *detectedPeaks);
		void smooth_null_values(std::vector<Common::TMeasuredValue *> *data);
		void moving_average(std::vector<Common::TMeasuredValue *> *data, int *windowSize);


	private:
		double calculateWindowFitness(std::vector<Common::TMeasuredValue *> *data, int startIndex, int endIndex);
		void detectPeakInDay(std::vector<Common::TMeasuredValue *> *data, std::vector<PeakPeakDetector::Peak> *peaks, int * windowSize);
			//projdeme fitness hodnoty okenek od startIndex do endIndex a vratime index nejlepsiho okenka
		int intervalMaxValueIndex(unsigned int startIndex, unsigned int endIndex, std::vector<double> *fitnessValues);
	};
}

