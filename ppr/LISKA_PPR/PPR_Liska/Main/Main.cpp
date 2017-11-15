// Main.cpp : Defines the entry point for the console application.
//
#include "InputParser.h"
#include "DataLoader.h"
#include "SVGExporter.h"
#include "PeakDetector.h"
#include "tbb\tbb.h"

void printHelp() 
{
	//TODO: Print help
	std::cout << "Chyba pri vstupnich parametrech" << std::endl;
}


int main(int argc, char* argv[])
{
	Parser::InputParser parser = Parser::InputParser(&argc, argv);
	int  windowSize = 48;
	int smoothSize = 10;

	if (parser.areParamstOk())
	{
		DataLoader::DataLoader dataLoader = DataLoader::DataLoader(parser.getDbPath());
		std::vector<int> segmentIds = std::vector<int>();
		std::vector<std::vector<TMeasuredValue*>> data = std::vector<std::vector<TMeasuredValue*>>();

		dataLoader.getSegmentIds(&segmentIds);
		SVGExporter::SVGExporter exporter = SVGExporter::SVGExporter();

		for (unsigned int i = 0; i < segmentIds.size(); i++)
		{
			std::vector<TMeasuredValue*> values;
			dataLoader.loadData(&values, &(segmentIds.at(i)));
			if (values.size() > 0)
			{
				std::vector<PeakPeakDetector::Peak> peaks;
				PeakDetector::PeakDetector detector = PeakDetector::PeakDetector();
				detector.smooth_null_values(&values);
				detector.moving_average(&values, &smoothSize);
				detector.detectPeaks(&values, &windowSize, &peaks);
				exporter.exportToSvg(parser.getExportPath(), &(values), &segmentIds.at(i), &peaks);
			}
		}
	}
	else 
	{
		printHelp();
	}
	std::cout << "Hotovo" << std::endl;
	getchar();
    return 0;
}

