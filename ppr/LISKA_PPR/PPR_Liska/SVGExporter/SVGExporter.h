#pragma once

#include <vector>
#include <string>

#include "TMeasuredValue.h"
#include "simple_svg_1.0.0.hpp"
#include "Peak.h"
#include "SegmentDay.h"

#ifdef  SVGEXPORTERRDLL_EXPORTS 
/*Enabled as "export" while compiling the dll project*/
#define SVGEXPORTERDLLEXPORT __declspec(dllexport)  
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define SVGEXPORTERDLLEXPORT __declspec(dllimport)  
#endif

namespace SVGExporter {

	class SVGEXPORTERDLLEXPORT SVGExporter
	{
	public:
		SVGExporter();
		void exportToSvg(std::string path, std::vector<Common::SegmentDay>  *days, int * segmentId, std::vector<std::vector<PeakPeakDetector::Peak>> *peaks, bool inOne);
	private:

		void allInOneGraph(std::string path, std::vector<Common::SegmentDay>  *days, int * segmentId, std::vector<std::vector<PeakPeakDetector::Peak>> *peaks);
		void oneToOneGraph(std::string path, std::vector<Common::SegmentDay>  *days, int * segmentId, std::vector<std::vector<PeakPeakDetector::Peak>> *peaks);


		void printXAxis(svg::Document *doc, const double * pixelPerMmol, const double *maxMmolGridValue, double *yOffset);
		void printYAxis(svg::Document *doc, std::vector<Common::TMeasuredValue*>  *values, double *yOffset);

		void printArrows(svg::Document *doc, double *yOffset);
		void printLegend(svg::Document *doc, double *yOffset);
		void printPeaks(svg::Document *doc, std::vector<Common::TMeasuredValue*>  *values, std::vector<PeakPeakDetector::Peak> *peaks, const double * columnWidth, const double * pixelPerMmol, double *yOffset);
		void printData(svg::Document *doc, std::vector<Common::TMeasuredValue*>  *values, const double * columnWidth, const double *pixelPerMol, double *yOffset);
		double getMaxIst(std::vector<Common::TMeasuredValue*>  *values);
		std::string getFileName(int * segmentId, std::string path);
	};

}

