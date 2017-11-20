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
	//int y = 5;
	//for (int i = 0; i < 10; i++)
	//{
	//	runSerial(params);
	//}
	//}
	double xTimes = 1000;
	double serial = 0;
	double perDay = 0;
	double notPerDay = 0;
	for (size_t i = 0; i < xTimes; i++)
	{
		std::cout << std::to_string(i) << std::endl;
		notPerDay += runParallel(params, false);
		perDay += runParallel(params, true);
		serial += runSerial(params);
	}

	std::cout << "Paralel per day" << std::endl;
	std::cout << std::to_string((perDay/ xTimes)*1000) << std::endl;
	std::cout << "Paralel not per day" << std::endl;
	std::cout << std::to_string((notPerDay/ xTimes) * 1000) << std::endl;
	std::cout << "Serial" << std::endl;
	std::cout << std::to_string((serial / xTimes) * 1000) << std::endl;

	getchar();
	int x = 5;

}

double runSerial(Parser::InputParser *params)
{
	int windowSize = (*params).getWindowSize();
	DataLoader::DataLoader dataLoader = DataLoader::DataLoader((*params).getDbPath());
	std::shared_ptr<std::vector<std::shared_ptr<Common::Segment>>> values = std::shared_ptr<std::vector<std::shared_ptr<Common::Segment>>>();
	dataLoader.loadData(values);
	tbb::tick_count before = tbb::tick_count::now();
	for (unsigned int i = 0; i < (*values).size(); i++)
	{
		PeakDetector::PeakDetector detector = PeakDetector::PeakDetector(false);
		if (values.get()->at(i).get()->getSegmentDays().get() != NULL) {
			std::shared_ptr<Common::SegmentDays> days = values.get()->at(i).get()->getSegmentDays();
			std::shared_ptr< std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>> peaks = std::shared_ptr< std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>();
			detector.detectPeaks(days, &windowSize, peaks);
			//MySVG::exportToSvg((*params).getExportPath(), values.get()->at(i), peaks, true);
		}
	}
	tbb::tick_count after = tbb::tick_count::now();
	return (after - before).seconds();
}


double runParallel(Parser::InputParser *params, bool dayParalelism)
{
	tbb::task_scheduler_init init;

	int windowSize = (*params).getWindowSize();
	DataLoader::DataLoader dataLoader = DataLoader::DataLoader((*params).getDbPath());
	std::shared_ptr<std::vector<std::shared_ptr<Common::Segment>>> values = std::shared_ptr<std::vector<std::shared_ptr<Common::Segment>>>();
	dataLoader.loadData(values);
	tbb::tick_count before = tbb::tick_count::now();

	tbb::parallel_for(size_t(0), (*values).size(), [&](size_t i) {
		PeakDetector::PeakDetector detector = PeakDetector::PeakDetector(false);

		if (values.get()->at(i).get()->getSegmentDays().get() != NULL) 
		{
			std::shared_ptr<Common::SegmentDays> days = values.get()->at(i).get()->getSegmentDays();
			std::shared_ptr< std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>> peaks = std::shared_ptr< std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>();
			detector.detectPeaks(days, &windowSize, peaks);
			//MySVG::exportToSvg((*params).getExportPath(), values.get()->at(i), peaks, true);
		}
	});

	tbb::tick_count after = tbb::tick_count::now();
	return (after - before).seconds();

}
void runOnGraphics(Parser::InputParser *params, std::vector<int> *segmentIds)
{

}

