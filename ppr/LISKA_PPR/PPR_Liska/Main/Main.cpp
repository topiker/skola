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
typedef std::chrono::high_resolution_clock Clock;




int main(int argc, char* argv[])
{
	Parser::InputParser parser = Parser::InputParser(&argc, argv);

	if (parser.areParamstOk())
	{
		runSolution(&parser);
	}
	else
	{
		printHelp();
	}

	//getchar();
	return 0;
}


void printHelp()
{
	//TODO: Print help
	std::cout << "Program slouzi k detekci vyznamnych odchylek ve vstupnich datech." << std::endl;
	std::cout << std::endl;
	std::cout << "-db [cesta k databazi]" << std::endl;
	std::cout << "-useGpu [0 - ne, 1 ano]" << std::endl;
	std::cout << "-outputDir [cesta k adresari pro export]" << std::endl;
	std::cout << std::endl;

}

void runSolution(Parser::InputParser *params)
{
	//if ((*params).isGpu())
	//{
	//	runOnGraphics(params, &segmentIds);
	//}
	//else if ((*params).isParallel())
	//{
	//	runParallel(params, &segmentIds,true);
	//}
	//else
	//{
		runSerial(params);
	//}
	double xTimes = 100;
	long long perDay = 0;
	long long notPerDay = 0;
	for (size_t i = 0; i < xTimes; i++)
	{
		std::cout << std::to_string(i) << std::endl;
		notPerDay += runParallel(params, false);
		perDay += runParallel(params, true);
	}

	std::cout << "Paralel per day" << std::endl;
	std::cout << std::to_string(perDay/ xTimes) << std::endl;
	std::cout << "Paralel not per day" << std::endl;
	std::cout << std::to_string(notPerDay/ xTimes) << std::endl;

	getchar();


}

void runSerial(Parser::InputParser *params)
{
	int windowSize = (*params).getWindowSize();
	DataLoader::DataLoader dataLoader = DataLoader::DataLoader((*params).getDbPath());
	std::vector<Common::Segment> values;
	dataLoader.loadData(&values);
	auto t1 = Clock::now();
	for (unsigned int i = 0; i < (values).size(); i++)
	{
		if ((values).at(i).getSegmentDays() != NULL) {
			Common::SegmentDays *days = values.at(i).getSegmentDays();
			std::vector<std::vector<PeakPeakDetector::Peak>> peaks;
			PeakDetector::PeakDetector detector = PeakDetector::PeakDetector(false);
			detector.detectPeaks(days, &windowSize, &peaks);
			//MySVG::exportToSvg((*params).getExportPath(), &(values).at(i), &peaks, false);
			//(values).at(i).clearAfterMe();
		}
	}

	auto t2 = Clock::now();

	std::cout << "Delta t2-t1: "
		<< (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000)
		<< " miliseconds" << std::endl;
}


long long runParallel(Parser::InputParser *params, bool dayParalelism)
{
	tbb::task_scheduler_init init;

	int windowSize = (*params).getWindowSize();
	DataLoader::DataLoader dataLoader = DataLoader::DataLoader((*params).getDbPath());
	std::vector<Common::Segment> values;
	dataLoader.loadData(&values);
	
	auto t1 = Clock::now();
	tbb::parallel_for(size_t(0), (values).size(), [&](size_t i) {
		if ((values).at(i).getSegmentDays() != NULL) {
			Common::SegmentDays *days = values.at(i).getSegmentDays();
			std::vector<std::vector<PeakPeakDetector::Peak>> peaks;
			PeakDetector::PeakDetector detector = PeakDetector::PeakDetector(false);
			detector.detectPeaks(days, &windowSize, &peaks);
			//MySVG::exportToSvg((*params).getExportPath(), &(values).at(i), &peaks, false);
		}
	});

	auto t2 = Clock::now();

	return (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000);
	return 0;

}
void runOnGraphics(Parser::InputParser *params, std::vector<int> *segmentIds)
{

}

