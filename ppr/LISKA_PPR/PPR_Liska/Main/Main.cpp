// Main.cpp : Defines the entry point for the console application.
//
#include "InputParser.h"
#include "DataLoader.h"
#include "SVGExporter.h"
#include "PeakDetector.h"

void printHelp() 
{
	//TODO: Print help

}

int main(int argc, char* argv[])
{
	Parser::InputParser parser = Parser::InputParser(&argc, argv);
	int  windowSize = 60;

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
				std::vector<Peak> peaks;
				PeakDetector::PeakDetector detector = PeakDetector::PeakDetector();
				detector.smooth_null_values(&values);
				detector.detectPeaks(&values, &windowSize, &peaks);
				exporter.exportToSvg(parser.getExportPath(), &(values), &segmentIds.at(i));
			}
		}

	}
	else 
	{
		printHelp();
	}

    return 0;
}

