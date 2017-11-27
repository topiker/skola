#pragma once

#include "Segment.h"
#include "PeakDetector.h"
#include <simple_svg_1.0.0.hpp>

namespace MySVG {

		void exportToSvg(std::string path, Common::Segment*const segment,const std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& peaks, bool inOne);
		void allInOneGraph(std::string path, Common::Segment*const segment, const std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& peaks);
		void oneToOneGraph(std::string path, Common::Segment*const segment, const std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& peaks);

		void printYAxis(svg::Document *doc, std::vector<Common::TMeasuredValue*>* const values, double *yOffset);
		void printYAxisDay(svg::Document *doc, std::vector<Common::TMeasuredValue*>* const values, double *yOffset);

		void printXAxis(svg::Document *doc, const double * pixelPerMmol, const double *maxMmolGridValue, double *yOffset);


		void printArrows(svg::Document *doc, double *yOffset);
		void printLegend(svg::Document *doc, double *yOffset);

		void printData(svg::Document *doc, std::vector<Common::TMeasuredValue*>* const values, const double * columnWidth, const double *pixelPerMol, double *yOffset);
		void printPeaks(svg::Document *doc,  std::vector<Common::TMeasuredValue*>* const values, std::vector<std::shared_ptr<PeakPeakDetector::Peak>> *peaks, const double * columnWidth, const double * pixelPerMmol, double *yOffset);
		
		void printDataDay(svg::Document *doc, std::vector<Common::TMeasuredValue*>* const values, const double *pixelPerMol, double *yOffset);
		void printPeaksDay(svg::Document *doc, std::vector<Common::TMeasuredValue*>* const values, std::vector<std::shared_ptr<PeakPeakDetector::Peak>> *peaks, const double * pixelPerMmol, double *yOffset);

		double getMaxIst(std::vector<std::unique_ptr<Common::TMeasuredValue>>* const values);


		std::string getFileName(int * segmentId, std::string path);

}
