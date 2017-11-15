#pragma once
#include "TMeasuredValue.h"
#include "simple_svg_1.0.0.hpp"
#include <vector>
#include "Peak.h"
#include <string>

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
		void exportToSvg(std::string path, std::vector<TMeasuredValue*>  *values, int * segmentId, std::vector<PeakPeakDetector::Peak> *peaks);
	private:

		void printXAxis(svg::Document *doc, const double * pixelPerMmol, const double *maxMmolGridValue);
		void printYAxis(svg::Document *doc, std::vector<TMeasuredValue*>  *values);

		void printArrows(svg::Document *doc);
		void printLegend(svg::Document *doc);
		void printPeaks(svg::Document *doc, std::vector<TMeasuredValue*>  *values, std::vector<PeakPeakDetector::Peak> *peaks, const double * columnWidth, const double * pixelPerMmol);
		void printData(svg::Document *doc, std::vector<TMeasuredValue*>  *values, const double * columnWidth, const double *pixelPerMol);
		std::string getFileName(int * segmentId, std::string path);
	};

}

