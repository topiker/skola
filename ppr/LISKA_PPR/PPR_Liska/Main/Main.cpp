// Main.cpp : Defines the entry point for the console application.
//
#include "InputParser.h"
#include "DataLoader.h"
#include "SVGExporter.h"

void printHelp() 
{
	//TODO: Print help

}

int main(int argc, char* argv[])
{
	Parser::InputParser parser = Parser::InputParser(&argc, argv);

	if (parser.areParamstOk())
	{
		DataLoader::DataLoader dataLoader = DataLoader::DataLoader(parser.getDbPath());
		std::vector<int> segmentIds = std::vector<int>();
		std::vector<std::vector<TMeasuredValue*>> data = std::vector<std::vector<TMeasuredValue*>>();

		dataLoader.getSegmentIds(&segmentIds);
		for (unsigned int i = 0; i < segmentIds.size(); i++)
		{
			std::vector<TMeasuredValue*> values;
			dataLoader.loadData(&values, &(segmentIds.at(i)));
			data.push_back(values);
		}
		SVGExporter::SVGExporter exporter = SVGExporter::SVGExporter();
		for (unsigned int i = 0; i < data.size(); i++)
		{
			exporter.exportToSvg(parser.getExportPath(), &(data.at(i)));
		}
	}
	else 
	{
		printHelp();
	}

	getchar();
    return 0;
}

