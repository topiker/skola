#pragma once

#include "Segment.h"
#include "PeakDetector.h"
#include <simple_svg_1.0.0.hpp>

namespace MySVG {

		/// <summary>
		/// Hlavni funkce pro export do SVG, vstupem jsou data a okenka, jejich indexi musi sedet
		/// </summary>
		void exportToSvg(std::string path, Common::Segment*const segment,const std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& peaks, bool graphPerDay);
		/// <summary>
		/// Vykresli data do jednoho grafu
		/// </summary>
		void allInOneGraph(std::string path, Common::Segment*const segment, const std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& peaks);
		/// <summary>
		/// Vykresli dat tak, ze co den, to graf
		/// </summary>
		void oneToOneGraph(std::string path, Common::Segment*const segment, const std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& peaks);
		/// <summary>
		/// Vypise Y osu
		/// </summary>
		void printYAxis(svg::Document *doc, std::vector<Common::TMeasuredValue*>* const values, double *yOffset);
		/// <summary>
		/// Vypise Y osu pro den od 0 - 23:59
		/// </summary>
		void printYAxisDay(svg::Document *doc, double *yOffset);
		/// <summary>
		/// Vykresli x osu
		/// </summary>
		void printXAxis(svg::Document *doc, const double * pixelPerMmol, const double *maxMmolGridValue, double *yOffset);

		/// <summary>
		/// Vykresli sipky
		/// </summary>
		void printArrows(svg::Document *doc, double *yOffset);
		/// <summary>
		/// Vykresli legendu jednotlivych car
		/// </summary>
		void printLegend(svg::Document *doc, double *yOffset);
		/// <summary>
		/// Vykresli data do grafu
		/// </summary>
		void printData(svg::Document *doc, std::vector<Common::TMeasuredValue*>* const values, const double * columnWidth, const double *pixelPerMol, double *yOffset);
		/// <summary>
		/// Vykresli okenka do grafu
		/// </summary>
		void printPeaks(svg::Document *doc,  std::vector<Common::TMeasuredValue*>* const values, std::vector<std::shared_ptr<PeakPeakDetector::Peak>> *peaks, const double * columnWidth, const double * pixelPerMmol, double *yOffset);
		/// <summary>
		/// Vykresli data do grafu, ktery zacina 0:00 a konci 23:59
		/// </summary>
		void printDataDay(svg::Document *doc, std::vector<Common::TMeasuredValue*>* const values, const double *pixelPerMol, double *yOffset);
		/// <summary>
		/// Vykresli okenka do grafu, ktery zacina 0:00 a konci 23:59
		/// </summary>
		void printPeaksDay(svg::Document *doc, std::vector<Common::TMeasuredValue*>* const values, std::vector<std::shared_ptr<PeakPeakDetector::Peak>> *peaks, const double * pixelPerMmol, double *yOffset);

		/// <summary>
		/// Ze vstupnich dat ziska maximalni IST hodnotu
		/// </summary>
		double getMaxIst(std::vector<std::unique_ptr<Common::TMeasuredValue>>* const values);

		/// <summary>
		/// Funkce vraci nazev souboru ve tvaru segnent_segmentId.svg
		/// </summary>
		std::string getFileName(int * segmentId, std::string path);
		/// <summary>
		/// Testuje, zda slozka existuje, pokud ne, zalozi ji
		/// </summary>
		void checkOrCreatedirectory(std::string path);

}
