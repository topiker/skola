#pragma once

#include "Segment.h"
#include "PeakDetector.h"
#include <simple_svg_1.0.0.hpp>

namespace MySVG {

		void exportToSvg(std::string path,const std::shared_ptr<Common::Segment>& segment,const std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& peaks, bool inOne);
		void allInOneGraph(std::string path, const std::shared_ptr<Common::Segment>& segment, const std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& peaks);
		void oneToOneGraph(std::string path, const std::shared_ptr<Common::Segment>& segment, const std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& peaks);

		void printXAxis(svg::Document *doc, const double * pixelPerMmol, const double *maxMmolGridValue, double *yOffset);
		void printYAxis(svg::Document *doc,const std::shared_ptr<std::vector<std::shared_ptr<Common::TMeasuredValue>>>& values, double *yOffset);

		void printArrows(svg::Document *doc, double *yOffset);
		void printLegend(svg::Document *doc, double *yOffset);
		void printPeaks(svg::Document *doc, const std::shared_ptr<std::vector<std::shared_ptr<Common::TMeasuredValue>>>& values, std::vector<std::shared_ptr<PeakPeakDetector::Peak>> *peaks, const double * columnWidth, const double * pixelPerMmol, double *yOffset);
		void printData(svg::Document *doc, const std::shared_ptr<std::vector<std::shared_ptr<Common::TMeasuredValue>>>& values, const double * columnWidth, const double *pixelPerMol, double *yOffset);
		double getMaxIst(const std::shared_ptr<std::vector<std::shared_ptr<Common::TMeasuredValue>>>& values);
		std::string getFileName(int * segmentId, std::string path);

}
