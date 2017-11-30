#include "SVGExporter.h"
#include "SVGExporterConstants.h"
#include "DataHelper.h"
#include <direct.h>

namespace MySVG {

	namespace fs = std::experimental::filesystem;


	void exportToSvg(std::string path, Common::Segment* const segment, const std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& peaks, bool graphPerDay)
	{
		//std::cout << "SEGMENT: " << (*segment).getSegmentId() << std::endl;
		//std::cout << "----------------" << std::endl;
		//for (int i = 0; i < peaks.get()->size(); i++)
		//{
		//	std::cout << "DEN: " << i << std::endl;
		//	std::cout << "----------------" << std::endl;
		//	auto tmpPeaks = peaks.get()->at(i);
		//	for (int j = 0; j < tmpPeaks.size(); j++)
		//	{
		//		std::cout << tmpPeaks.at(j).get()->startIndex << " - " << tmpPeaks.at(j).get()->endIndex << std::endl;
		//	}
		//	std::cout << std::endl;
		//}
		//std::cout << std::endl;
		//std::cout << std::endl;
		//std::cout << std::endl;
		//std::cout << std::endl;
		checkOrCreatedirectory(path);
		if (graphPerDay)
		{
			MySVG::oneToOneGraph(path, segment, peaks);
		}
		else
		{
			MySVG::allInOneGraph(path, segment, peaks);
		}
	}

	void oneToOneGraph(std::string path, Common::Segment* const segment, const std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& peaks)
	{
		auto days = (*segment).getSegmentDays()->getDays();
		int segmentId = (*segment).getSegmentId();
		svg::Dimensions dimensions(width, height*(days->size()));
		svg::Document doc(getFileName((&segmentId), path), svg::Layout(dimensions, svg::Layout::BottomLeft));
		double xOffset = 0;

		
		double maxMmolValue = 0;
		double currentMax;
		for (size_t i = 0; i < (*days).size(); i++)
		{
			auto values = (*days).at(i).get()->getData();
			currentMax = getMaxIst(values);
			maxMmolValue = (currentMax > maxMmolValue) ? currentMax : maxMmolValue;
		}

		for (size_t i = 0; i < (*days).size(); i++)
		{
			double yOffset = height * ((*days).size() - (i + 1));
			auto peaksCurrent = (peaks).get()->at(i);

			double maxMmolGridValue = ((double)(int)maxMmolValue) + 1;

			std::vector<Common::TMeasuredValue*> graphData = std::vector<Common::TMeasuredValue*>();
			auto current = (*days).at(i).get();
			auto currentData = (current->getData());
			for (size_t j = 0; j < currentData->size(); j++)
			{
				graphData.push_back(currentData->at(j).get());
			}

			//Vypocet hodnoty na pixel
			double pixelPerMmol = chartHeight / maxMmolGridValue;
			MySVG::printXAxis(&doc, &pixelPerMmol, &maxMmolGridValue, &yOffset);
			MySVG::printYAxisDay(&doc,&yOffset);
			MySVG::printDataDay(&doc, &graphData, &pixelPerMmol, &yOffset);
			MySVG::printLegend(&doc, &yOffset);
			MySVG::printPeaksDay(&doc, &graphData, &peaksCurrent, &pixelPerMmol, &yOffset);
			MySVG::printArrows(&doc, &yOffset);
			xOffset += (graphData).size();
		}


		doc.save();
	}



	void allInOneGraph(std::string path, Common::Segment* const segment, const std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& peaks)
	{
		auto days = (*segment).getSegmentDays()->getDays();
		int segmentId = (*segment).getSegmentId();

		svg::Dimensions dimensions(width, height);
		svg::Document doc(getFileName((&segmentId), path), svg::Layout(dimensions, svg::Layout::BottomLeft));
		double yOffset = 0;

		double maxMmolValue = -DBL_MAX;
		double currentMol = 0;
		size_t dataSize = 0;
		for (size_t i = 0; i < (*days).size(); i++)
		{
			auto current = (*days).at(i).get();
			currentMol = MySVG::getMaxIst((current->getData()));
			if (currentMol > maxMmolValue)
			{
				maxMmolValue = currentMol;
			}
			dataSize += current->getData()->size();

		}

		double maxMmolGridValue = ((double)(int)maxMmolValue) + 1;
		//Vypocet hodnoty na pixel
		double pixelPerMmol = chartHeight / maxMmolGridValue;
		//Vypocet sirky sloupce, tak aby se nam vesly do obrazku
		double columnWidth = (chartWidth) / dataSize;
		std::vector<Common::TMeasuredValue*> graphData = std::vector<Common::TMeasuredValue*>();

		for (size_t i = 0; i < (*days).size(); i++)
		{
			auto current = (*days).at(i).get();
			auto currentData = (current->getData());
			for (size_t j = 0; j < currentData->size(); j++)
			{
				graphData.push_back(currentData->at(j).get());
			}
		}
		std::vector<std::shared_ptr<PeakPeakDetector::Peak>> peaksData;

		MySVG::printXAxis(&doc, &pixelPerMmol, &maxMmolGridValue, &yOffset);
		MySVG::printYAxis(&doc, &graphData, &yOffset);
		MySVG::printData(&doc, &graphData, &columnWidth, &pixelPerMmol, &yOffset);

		size_t xOffset = 0;
		for (size_t i = 0; i < (*peaks).size(); i++)
		{
			auto current = (*peaks).at(i);
			auto currentData = (current);
			for (size_t j = 0; j < currentData.size(); j++)
			{
				auto currentPeak = std::make_shared<PeakPeakDetector::Peak>(*(currentData.at(j).get()));
				currentPeak.get()->startIndex = currentPeak.get()->startIndex + xOffset;
				currentPeak.get()->endIndex = currentPeak.get()->endIndex + xOffset;
				peaksData.push_back(currentPeak);
			}
			xOffset += (*days).at(i).get()->getData()->size();
		}

		MySVG::printPeaks(&doc, &graphData, &peaksData, &columnWidth, &pixelPerMmol, &yOffset);
		MySVG::printLegend(&doc, &yOffset);
		MySVG::printArrows(&doc, &yOffset);

		doc.save();
	}

	double getMaxIst( std::vector<std::unique_ptr<Common::TMeasuredValue>>*const values)
	{
		double maxMmolValue = -DBL_MAX;
		for (size_t i = 0; i < (*values).size(); i++)
		{
			if ((*values).at(i)->ist > maxMmolValue)
			{
				maxMmolValue = (*values).at(i)->ist;
			}
		}
		return maxMmolValue;
	}

	void printXAxis(svg::Document *doc, const double * pixelPerMmol, const double *maxMmolGridValue, double *yOffset)
	{
		svg::Polyline polyline_axis(svg::Stroke(2, svg::Color::Black));
		polyline_axis << svg::Point(chartMinX, chartMaxY + (*yOffset)) << svg::Point(chartMinX, chartMinY + (*yOffset)) << svg::Point(chartMaxX, chartMinY + (*yOffset));
		(*doc) << polyline_axis;

		(*doc) << svg::Text(svg::Point(canvasMinX, chartMinY + (*yOffset) + (*maxMmolGridValue) * (*pixelPerMmol) - fontSize / 2), "[mmol/L]", svg::Color::Black, svg::Font(fontSize, "Verdana"));

		for (unsigned int i = 1; i < (*maxMmolGridValue); i++)
		{
			(*doc) << svg::Text(svg::Point(canvasMinX + 20, chartMinY + (*yOffset) + i * (*pixelPerMmol) - fontSize / 2), std::to_string(i), svg::Color::Black, svg::Font(fontSize, "Verdana"));
			(*doc) << svg::Line(svg::Point(chartMinX, chartMinY + (*yOffset) + i * (*pixelPerMmol)), svg::Point(chartMaxX, chartMinY + (*yOffset) + i * (*pixelPerMmol)), svg::Stroke(1, svg::Color::Grey));
		}

	}
	void printArrows(svg::Document *doc, double *yOffset)
	{
		svg::Polyline leftArrow(svg::Stroke(2, svg::Color::Black));
		leftArrow << svg::Point(chartMinX - 8, chartMaxY + (*yOffset) - 10) << svg::Point(chartMinX, chartMaxY + (*yOffset) + 5) << svg::Point(chartMinX + 8, chartMaxY + (*yOffset) - 10);
		(*doc) << leftArrow;

		svg::Polyline rightArrow(svg::Stroke(2, svg::Color::Black));
		rightArrow << svg::Point(chartMaxX - 10, chartMinY + (*yOffset) - 8) << svg::Point(chartMaxX + 5, chartMinY + (*yOffset)) << svg::Point(chartMaxX - 10, chartMinY + (*yOffset) + 8);
		(*doc) << rightArrow;
	}
	void printLegend(svg::Document *doc, double *yOffset)
	{
		double legendStartY = chartMinY + (*yOffset) + (chartHeight) / 2;
		(*doc) << svg::Text(svg::Point(chartMaxX + 25, legendStartY + fontSize), "Namereno", svg::Color::Black, svg::Font(fontSize, "Verdana"));
		(*doc) << svg::Line(svg::Point(chartMaxX + double(legendLineWidth / 2), legendStartY), svg::Point(canvasMaxX - double(legendLineWidth / 2), legendStartY), svg::Stroke(3, svg::Color::Black));

		(*doc) << svg::Text(svg::Point(chartMaxX + 20, legendStartY - fontSize - legendItemMargin), "Detekovano", svg::Color::Black, svg::Font(fontSize, "Verdana"));
		(*doc) << svg::Line(svg::Point(chartMaxX + double(legendLineWidth / 2), legendStartY - fontSize * 2 - legendItemMargin), svg::Point(canvasMaxX - double(legendLineWidth / 2), legendStartY - fontSize * 2 - legendItemMargin), svg::Stroke(4, svg::Color::Red));
		(*doc) << svg::Line(svg::Point(chartMaxX + double(legendLineWidth / 2), legendStartY - fontSize * 2 - legendItemMargin), svg::Point(canvasMaxX - double(legendLineWidth / 2), legendStartY - fontSize * 2 - legendItemMargin), svg::Stroke(2, svg::Color::White));
	}

	void printPeaks(svg::Document *doc, std::vector<Common::TMeasuredValue*>*const values, std::vector<std::shared_ptr<PeakPeakDetector::Peak>> *peaks, const double * columnWidth, const double * pixelPerMmol, double *yOffset)
	{
		for (size_t i = 0; i < (*peaks).size(); i++)
		{
			svg::Polyline peakLineBackground(svg::Stroke(4, svg::Color::Red));
			svg::Polyline peakLine(svg::Stroke(2, svg::Color::White));


			for (size_t j = (*peaks).at(i).get()->startIndex; j < (*peaks).at(i).get()->endIndex; j++)
			{
				peakLineBackground << svg::Point(chartMinX + j*(*columnWidth), (*pixelPerMmol) * (*values).at(j)->ist + chartMinY + (*yOffset)), 10, svg::Fill(svg::Color::Red);
				peakLine << svg::Point(chartMinX + j*(*columnWidth), (*pixelPerMmol) * (*values).at(j)->ist + chartMinY + (*yOffset)), 10, svg::Fill(svg::Color::Red);
			}
			(*doc) << peakLineBackground;
			(*doc) << peakLine;
		}
	}

	void printPeaksDay(svg::Document *doc, std::vector<Common::TMeasuredValue*>* const values, std::vector<std::shared_ptr<PeakPeakDetector::Peak>> *peaks, const double * pixelPerMmol, double *yOffset)
	{
		//pocet minut za den
		unsigned int minutesPerDay = 1440;
		double minuteWidth = chartWidth / (double)minutesPerDay;
		double currentMinutesFromStart = 0;
		for (size_t i = 0; i < (*peaks).size(); i++)
		{
			svg::Polyline peakLineBackground(svg::Stroke(4, svg::Color::Red));
			svg::Polyline peakLine(svg::Stroke(2, svg::Color::White));


			for (size_t j = (*peaks).at(i).get()->startIndex; j < (*peaks).at(i).get()->endIndex; j++)
			{
				currentMinutesFromStart = (*values).at(j)->hour * 60 + (*values).at(j)->minutes;
				peakLineBackground << svg::Point(chartMinX + currentMinutesFromStart*minuteWidth, (*pixelPerMmol) * (*values).at(j)->ist + chartMinY + (*yOffset)), 10, svg::Fill(svg::Color::Red);
				peakLine << svg::Point(chartMinX + currentMinutesFromStart*minuteWidth, (*pixelPerMmol) * (*values).at(j)->ist + chartMinY + (*yOffset)), 10, svg::Fill(svg::Color::Red);
			}
			(*doc) << peakLineBackground;
			(*doc) << peakLine;
		}
	}

	void printDataDay(svg::Document *doc, std::vector<Common::TMeasuredValue*>* const values, const double *pixelPerMol, double *yOffset)
	{
		svg::Polyline polyline_chart(svg::Stroke(2, svg::Color::Black));

		//pocet minut za den
		unsigned int minutesPerDay = 1440;
		double minuteWidth = chartWidth / (double)minutesPerDay;
		//Vypocet poctu minut na jeden dilek - 24 hodin

		double minutesFromDayBegin = 0;
		for (size_t i = 0; i < (*values).size(); i++)
		{
			minutesFromDayBegin = (values)->at(i)->hour * 60 + (values)->at(i)->minutes;
			polyline_chart << svg::Point(chartMinX + minutesFromDayBegin*minuteWidth, (*pixelPerMol) * (*values).at(i)->ist + chartMinY + (*yOffset));
		}

		(*doc) << polyline_chart;

	}

	void printData(svg::Document *doc, std::vector<Common::TMeasuredValue*>* const values, const double * columnWidth, const double *pixelPerMol, double *yOffset)
	{
		svg::Polyline polyline_chart(svg::Stroke(2, svg::Color::Black));
		//svg::Polyline smoothed_polyline(svg::Stroke(2, svg::Color::Blue));


		for (size_t i = 0; i < (*values).size(); i++)
		{
			polyline_chart << svg::Point(chartMinX + i*(*columnWidth), (*pixelPerMol) * (*values).at(i)->ist + chartMinY + (*yOffset));
			//smoothed_polyline << svg::Point(chartMinX + i*(*columnWidth), (*pixelPerMol) * (*values).at(i)->smoothedValue + chartMinY + (*yOffset));
		}
		(*doc) << polyline_chart;
		//(*doc) << smoothed_polyline;

	}

	void printYAxis(svg::Document *doc,  std::vector<Common::TMeasuredValue*>* const values, double *yOffset)
	{
		if ((*values).size() > 0)
		{
			//pocet minut za den
			unsigned int minutesPerDay = 1440;
			//Rozdil posledniho a prvni v minutach
			unsigned int dataMinutes = (int)(((*values).at((*values).size() - 1)->measureDate - (*values).at(0)->measureDate)* minutesPerDay);

			unsigned int frequency = dataMinutes / 25; // kazdych 90 minut bude cara
			double pixelPerMinute = chartWidth / dataMinutes;

			double currentTime = (*values).at(0)->measureDate*minutesPerDay;
			int hours;
			int minutes;
			for (unsigned int i = 0; i < dataMinutes; i += frequency)
			{

				(*doc) << svg::Line(svg::Point(chartMinX + (i*pixelPerMinute), chartMinY + (*yOffset)), svg::Point(chartMinX + (i*pixelPerMinute), chartMaxY + (*yOffset)), svg::Stroke(1, svg::Color::Grey));
				//TODO: Doplnit nuly
				hours = ((int)(currentTime / 60)) % 24;
				minutes = (((int)currentTime) % 60);

				(*doc) << svg::Text(svg::Point(chartMinX + (i*pixelPerMinute) - textShiftLeft, chartMinY + (*yOffset) - textShiftDown), std::to_string(hours) + ":" + ((minutes > 9) ? std::to_string(minutes) : std::string("0") + (std::to_string(minutes))), svg::Color::Black, svg::Font((int)(fontSize*.9), "Verdana"));
				currentTime += frequency;
			}
		}

	}

	void printYAxisDay(svg::Document *doc, double *yOffset)
	{
		double hourWidth = chartWidth/24;
		for (int i = 1; i < 24; i++)
		{
			(*doc) << svg::Line(svg::Point(chartMinX + (i*hourWidth), chartMinY + (*yOffset)), svg::Point(chartMinX + (i*hourWidth), chartMaxY + (*yOffset)), svg::Stroke(1, svg::Color::Grey));
			(*doc) << svg::Text(svg::Point(chartMinX + (i*hourWidth) - textShiftLeft, chartMinY + (*yOffset) - textShiftDown), ((i > 9) ? std::to_string(i) : std::string("0") + (std::to_string(i))) + ":00", svg::Color::Black, svg::Font((int)(fontSize*.9), "Verdana"));

		}
	}




	std::string getFileName(int * segmentId, std::string path)
	{
		std::string exportFileName(path);
		std::string name("segment_");
		std::string extension(".svg");
		std::string segmentName = std::to_string(*segmentId);
		name = exportFileName + name + segmentName + extension;
		return name;
	}


	void checkOrCreatedirectory(std::string path)
	{
		if (_mkdir(path.c_str())) {
			//already exists 
		}
	}


}
