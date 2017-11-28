// Main.cpp : Defines the entry point for the console application.
//
#include "InputParser.h"
#include "DataLoader.h"
#include "SVGExporter.h"
#include "PeakDetector.h"
#include <tbb\tbb.h>
#include "Segment.h"
#include "Main.h"
#include <iostream>
#include <chrono>
#include "PeakDetectorAMP.h"
#include "TMeasuredValue.h"
typedef std::chrono::high_resolution_clock Clock;




int main(int argc, char* argv[])
{

	Parser::InputParser parser = Parser::InputParser(&argc, argv);

	if (parser.areParamstOk())
	{
		parser.printInfo();
		for (int i = 0; i < 1; i++)
		{
			runSolution(&parser);
		}
	}
	else
	{
		parser.printHelp();
	}
	getchar();
	return 0;
}


void runSolution(Parser::InputParser *params)
{
	DataLoader::DataLoader dataLoader = DataLoader::DataLoader((*params).getDbPath());
	std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>> values = std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>();

	dataLoader.loadData(values);
	std::vector<std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>> allPeaks = std::vector<std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>>(values.get()->size());

	double xTimes = 1;
	double serial = 0;
	double perDay = 0;
	double gpu = 0;
	tbb::tick_count before = tbb::tick_count::now();

	for (size_t i = 0; i < xTimes; i++)
	{

		if ((*params).isGpu())
		{
			gpu += runOnGraphics(params, values, allPeaks);
		}
		else if ((*params).isParallel())
		{
			perDay += runParallel(params, values, allPeaks);
		}
		else
		{
			serial += runSerial(params, values, allPeaks);
		}

		if ((*params).isDoExport())
		{
			//printToSvg(params, values, allPeaks);
		}

		//printAsCSV(values, allPeaks);
	}

	tbb::tick_count after = tbb::tick_count::now();

	double together = serial + gpu + perDay;

	std::cout << "Doba pouze vypoctu:" << (together*1000)/ xTimes << std::endl;
	std::cout << "Doba celkem bez nacteni dat:" << ((after - before).seconds()*1000)/xTimes << std::endl;

}

void runComputation(std::unique_ptr<Common::Segment>& segment, bool dayParalelism,int *windowSize, std::shared_ptr< std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& detectedPeaks)
{
	if (segment.get()->getSegmentDays() != nullptr)
	{
		Common::SegmentDays* days = segment.get()->getSegmentDays();
		detectedPeaks = std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>();
		PeakDetector::detectPeaks(days, windowSize, detectedPeaks, dayParalelism);
	}
}

double runSerial(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data, std::vector<std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>>& allPeaks)
{
	int windowSize = (*params).getWindowSize();

	tbb::tick_count before = tbb::tick_count::now();
	for (unsigned int i = 0; i < (data).get()->size(); i++)
	{
		std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>> detectedPeaks;
		runComputation(data.get()->at(i), params->isParalelismPerDay(),&windowSize, detectedPeaks);
		allPeaks[i] = detectedPeaks;
	}
	tbb::tick_count after = tbb::tick_count::now();
	return (after - before).seconds();
}


double runParallel(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data, std::vector<std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>>& allPeaks)
{
	tbb::task_scheduler_init init;
	int windowSize = (*params).getWindowSize();
	tbb::tick_count before = tbb::tick_count::now();
	tbb::parallel_for(size_t(0), (data).get()->size(), [&](size_t i) {
		std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>> detectedPeaks;
		runComputation(data.get()->at(i), params->isParalelismPerDay(),&windowSize,detectedPeaks);
		allPeaks[i] = detectedPeaks;
	});
	tbb::tick_count after = tbb::tick_count::now();
	return (after - before).seconds();

}

double runOnGraphics(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data, std::vector<std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>> &allPeaks)
{
	return PeakDetectorAMP::runOnGraphics(params, data, allPeaks);
}

double printToSvg(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data, std::vector<std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>> &allPeaks)
{
	tbb::tick_count before = tbb::tick_count::now();
	for (unsigned int i = 0; i < (data).get()->size(); i++)
	{
		if (data.get()->at(i).get()->getSegmentDays() != nullptr)
		{
			auto detectedPeaks = allPeaks.at(i);
			MySVG::exportToSvg((*params).getExportPath(), data.get()->at(i).get(), detectedPeaks, (*params).isGraphPerDay());
		}
	}
	tbb::tick_count after = tbb::tick_count::now();
	return (after - before).seconds();
}
double printAsCSV(const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data, std::vector<std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>> &allPeaks)
{
	tbb::tick_count before = tbb::tick_count::now();
	
	std::cout << "SEGMENTID;DEN;HODINASTARTU:MINUTASTARTU;HODINAKONCE:MINUTAKONCE"<<std::endl;
	int startHours = 0;
	int startMinutes = 0;
	int endHours = 0;
	int endMinutes = 0;
	int segmentId = 0;
	for (unsigned int i = 0; i < (data).get()->size(); i++)
	{
		auto currentSegment = data.get()->at(i).get();
		segmentId = data.get()->at(i).get()->getSegmentId();
		if (currentSegment->getSegmentDays() != nullptr)
		{
			auto detectedPeaks = allPeaks.at(i);
			size_t currentDayCount = 0;
			for (unsigned int j = 0; j < currentSegment->getSegmentDays()->getDaysSize(); j++)
			{
				
				for each (auto dayPeak in detectedPeaks.get()->at(j))
				{
					auto startData = currentSegment->getSegmentDays()->getDays()->at(j).get()->getData()->at(dayPeak.get()->startIndex - currentDayCount).get();
					auto endData = currentSegment->getSegmentDays()->getDays()->at(j).get()->getData()->at(dayPeak.get()->endIndex - currentDayCount).get();
					startHours = startData->hour;
					startMinutes = startData->minutes;
					endHours = endData->hour;
					endMinutes = endData->minutes;
					std::cout << segmentId << ";" << std::to_string(j + 1) << "," << startHours << ":" << startMinutes << ";" << endHours << ":" << endMinutes << std::endl;
				}
				currentDayCount += 0;// currentSegment->getSegmentDays()->getDays()->at(j).get()->getData()->size();
			}
		}
	}
	tbb::tick_count after = tbb::tick_count::now();
	return (after - before).seconds();
}

