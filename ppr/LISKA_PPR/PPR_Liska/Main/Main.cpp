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
	std::cout << "Program slouzi k detekci vyznamnych odchylek ve vstupnich datech." << std::endl;
	std::cout << std::endl;
	std::cout << "-db [cesta k databazi]" << std::endl;
	std::cout << "-useGpu [0 - ne, 1 ano]" << std::endl;
	std::cout << "-outputDir [cesta k adresari pro export]" << std::endl;
	std::cout << std::endl;

}


int main(int argc, char* argv[])
{
	Parser::InputParser parser = Parser::InputParser(&argc, argv);
	//24 = 2 hodiny
	int windowSize = 36;
	int smoothSize = 4;

	if (parser.areParamstOk())
	{
		DataLoader::DataLoader dataLoader = DataLoader::DataLoader(parser.getDbPath());
		std::vector<int> segmentIds = std::vector<int>();
		std::vector<std::vector<Common::TMeasuredValue*>> data = std::vector<std::vector<Common::TMeasuredValue*>>();

		dataLoader.getSegmentIds(&segmentIds);
		SVGExporter::SVGExporter exporter = SVGExporter::SVGExporter();

		for (unsigned int i = 0; i < segmentIds.size(); i++)
		{
			std::vector<Common::TMeasuredValue *> values;
			dataLoader.loadData(&values, &(segmentIds.at(i)));
			if (values.size() > 0)
			{
				std::vector<Common::SegmentDay> days;
				std::vector<std::vector<PeakPeakDetector::Peak>> peaks;
				PeakDetector::PeakDetector detector = PeakDetector::PeakDetector();
				detector.smooth_null_values(&values);
				detector.moving_average(&values, &smoothSize);
				dataLoader.splitIntoDays(&days, &values);
				detector.detectPeaks(&days,&windowSize,&peaks);
				exporter.exportToSvg(parser.getExportPath(), &(days), &segmentIds.at(i), &peaks,true);
			}
		}
	}
	else 
	{
		printHelp();
	}

	//getchar();
    return 0;
}

