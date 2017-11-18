// Main.cpp : Defines the entry point for the console application.
//
#include "InputParser.h"
#include "DataLoader.h"
#include "SVGExporter.h"
#include "PeakDetector.h"
#include "tbb\tbb.h"
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
	DataLoader::DataLoader dataLoader = DataLoader::DataLoader((*params).getDbPath());
	std::vector<int> segmentIds = std::vector<int>();
	std::vector<std::vector<Common::TMeasuredValue*>> data = std::vector<std::vector<Common::TMeasuredValue*>>();

	dataLoader.getSegmentIds(&segmentIds);

	if ((*params).isGpu())
	{
		runOnGraphics(params, &segmentIds);
	}
	else if ((*params).isParallel())
	{
		runParallel(params, &segmentIds,true);
	}
	else
	{
		runSerial(params, &segmentIds);
	}
	/*double xTimes = 100;
	long long perDay = 0;
	long long notPerDay = 0;
	for (size_t i = 0; i < xTimes; i++)
	{
		std::cout << std::to_string(i) << std::endl;
		notPerDay += runParallel(params, &segmentIds, false);
		perDay += runParallel(params, &segmentIds, true);
	}

	std::cout << "Paralel per day" << std::endl;
	std::cout << std::to_string(perDay/ xTimes) << std::endl;
	std::cout << "Paralel not per day" << std::endl;
	std::cout << std::to_string(notPerDay/ xTimes) << std::endl;*/


	//std::cout << "Parallel" << std::endl;
	//runParallel(params, &segmentIds, false);
	//std::cout << "Parallel day" << std::endl;
	//runParallel(params, &segmentIds, true);

	//std::cout << "Serial" << std::endl;
	//runSerial(params, &segmentIds);
	//std::cout << "Serial" << std::endl;
	//runSerial(params, &segmentIds);


}

void runSerial(Parser::InputParser *params, std::vector<int> *segmentIds)
{
	int windowSize = (*params).getWindowSize();
	DataLoader::DataLoader dataLoader = DataLoader::DataLoader((*params).getDbPath());
	auto t1 = Clock::now();
	for (unsigned int i = 0; i < (*segmentIds).size(); i++)
	{
		std::vector<Common::TMeasuredValue *> values;
		dataLoader.loadData(&values, &((*segmentIds).at(i)));
		if (values.size() > 0)
		{
			std::vector<Common::SegmentDay> days;
			std::vector<std::vector<PeakPeakDetector::Peak>> peaks;
			PeakDetector::PeakDetector detector = PeakDetector::PeakDetector(false);
			detector.smooth_null_values(&values);
			dataLoader.splitIntoDays(&days, &values);
			detector.detectPeaks(&days, &windowSize, &peaks);
			SVGExporter::SVGExporter exporter = SVGExporter::SVGExporter();
			exporter.exportToSvg((*params).getExportPath(), &(days), &(*segmentIds).at(i), &peaks, true);
			dataLoader.freeData(&values);
			values.clear();
		}
	}

	auto t2 = Clock::now();

	std::cout << "Delta t2-t1: "
		<< (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000)
		<< " nanoseconds" << std::endl;
}


long long runParallel(Parser::InputParser *params, std::vector<int> *segmentIds, bool dayParalelism)
{
	tbb::task_scheduler_init init;

	int windowSize = (*params).getWindowSize();
	
	auto t1 = Clock::now();

	tbb::parallel_for(size_t(0), (*segmentIds).size(), [&](size_t i) {
		DataLoader::DataLoader dataLoader = DataLoader::DataLoader((*params).getDbPath());
		std::vector<Common::TMeasuredValue *> values;
		dataLoader.loadData(&values, &((*segmentIds).at(i)));
		if (values.size() > 0)
		{ 
			std::vector<Common::SegmentDay> days;
			std::vector<std::vector<PeakPeakDetector::Peak>> peaks;
			PeakDetector::PeakDetector detector = PeakDetector::PeakDetector(dayParalelism);
			detector.smooth_null_values(&values);
			dataLoader.splitIntoDays(&days, &values);
			detector.detectPeaks(&days, &windowSize, &peaks);
			SVGExporter::SVGExporter exporter = SVGExporter::SVGExporter();
			exporter.exportToSvg((*params).getExportPath(), &(days), &(*segmentIds).at(i), &peaks, true);
			dataLoader.freeData(&values);
			values.clear();
		}
	});

	auto t2 = Clock::now();

	//std::cout << "Delta t2-t1: "
	//	<< (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000)
	//	<< " nanoseconds" << std::endl;

	return (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000);

}
void runOnGraphics(Parser::InputParser *params, std::vector<int> *segmentIds)
{

}

