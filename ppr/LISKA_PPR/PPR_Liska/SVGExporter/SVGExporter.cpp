#include "SVGExporter.h"
#include "SVGExporterConstants.h"
#include "DataHelper.h"
#include <ctime>

namespace SVGExporter {

	SVGExporter::SVGExporter() 
	{
	}

	void  SVGExporter::exportToSvg(std::string path, std::vector<Common::SegmentDay>  *days, int * segmentId, std::vector<std::vector<PeakPeakDetector::Peak>> *peaks, bool inOne)
	{
		if (inOne)
		{
		this->allInOneGraph(path, days, segmentId, peaks);
		}
		else 
		{
			this->oneToOneGraph(path, days, segmentId, peaks);
		}
	}

	void SVGExporter::oneToOneGraph(std::string path, std::vector<Common::SegmentDay>  *days, int * segmentId, std::vector<std::vector<PeakPeakDetector::Peak>> *peaks)
	{
		svg::Dimensions dimensions(width, height*(days->size()));
		svg::Document doc(getFileName(segmentId, path), svg::Layout(dimensions, svg::Layout::BottomLeft));
		double xOffset = 0;
		for (size_t i = 0; i < (*days).size(); i++)
		{
			double yOffset = height * ((*days).size()-(i+1));
			std::vector<Common::TMeasuredValue * > values = (*days).at(i).data;
			std::vector<PeakPeakDetector::Peak> peaksCurrent = (*peaks).at(i);
			double maxMmolValue = DataHelper::getMaxIstValue(&values);

			double maxMmolGridValue = ((double)(int)maxMmolValue) + 1;

			//Vypocet hodnoty na pixel
			double pixelPerMmol = chartHeight / maxMmolGridValue;
			//Vypocet sirky sloupce, tak aby se nam vesly do obrazku
			double columnWidth = (chartWidth) / (values).size();

			this->printXAxis(&doc, &pixelPerMmol, &maxMmolGridValue, &yOffset);
			this->printYAxis(&doc, &values, &yOffset);
			this->printData(&doc, &values, &columnWidth, &pixelPerMmol, &yOffset);
			this->printLegend(&doc, &yOffset);
			this->printPeaks(&doc, &values, &peaksCurrent, &columnWidth, &pixelPerMmol, &yOffset);
			this->printArrows(&doc, &yOffset);
			xOffset += values.size();
		}


		doc.save();
	}

	void SVGExporter::allInOneGraph(std::string path, std::vector<Common::SegmentDay>  *days, int * segmentId, std::vector<std::vector<PeakPeakDetector::Peak>> *peaks)
	{
		svg::Dimensions dimensions(width, height);
		svg::Document doc(getFileName(segmentId, path), svg::Layout(dimensions, svg::Layout::BottomLeft));
		double yOffset = 0;

		double maxMmolValue = -DBL_MAX;
		double currentMol = 0;
		size_t dataSize = 0;
		for (size_t i = 0; i < (*days).size(); i++)
		{
			auto current = (*days).at(i);
			currentMol = this->getMaxIst(&(current.data));
			if (currentMol > maxMmolValue)
			{
				maxMmolValue = currentMol;
			}
			dataSize += (current).data.size();

		}

		double maxMmolGridValue = ((double)(int)maxMmolValue) + 1;
		//Vypocet hodnoty na pixel
		double pixelPerMmol = chartHeight / maxMmolGridValue;
		//Vypocet sirky sloupce, tak aby se nam vesly do obrazku
		double columnWidth = (chartWidth) / dataSize;
		std::vector<Common::TMeasuredValue *> graphData;

		for (size_t i = 0; i < (*days).size(); i++)
		{
			auto current = (*days).at(i);
			auto currentData = (current.data);
			for (size_t j = 0; j < currentData.size(); j++)
			{
				graphData.push_back(currentData.at(j));
			}
		}
		std::vector<PeakPeakDetector::Peak> peaksData;

		this->printXAxis(&doc, &pixelPerMmol, &maxMmolGridValue, &yOffset);
		this->printYAxis(&doc, &graphData, &yOffset);
		this->printData(&doc, &graphData, &columnWidth, &pixelPerMmol,&yOffset);

		size_t xOffset = 0;
		for (size_t i = 0; i < (*peaks).size(); i++)
		{
			auto current = (*peaks).at(i);
			auto currentData = (current);
			for (size_t j = 0; j < currentData.size(); j++)
			{
				auto currentPeak = currentData.at(j);
				currentPeak.startIndex = currentPeak.startIndex + xOffset;
				currentPeak.endIndex = currentPeak.endIndex + xOffset;
				peaksData.push_back(currentPeak);
			}
			xOffset += (*days).at(i).data.size();
		}
		this->printPeaks(&doc, &graphData, &peaksData, &columnWidth, &pixelPerMmol,&yOffset);
		this->printLegend(&doc,&yOffset);
		this->printArrows(&doc,&yOffset);

		doc.save();
	}

	double SVGExporter::getMaxIst(std::vector<Common::TMeasuredValue*>  *values)
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

	void SVGExporter::printXAxis(svg::Document *doc, const double * pixelPerMmol, const double *maxMmolGridValue, double *yOffset)
	{
		svg::Polyline polyline_axis(svg::Stroke(2, svg::Color::Black));
		polyline_axis << svg::Point(chartMinX, chartMaxY+ (*yOffset)) << svg::Point(chartMinX, chartMinY+ (*yOffset)) << svg::Point(chartMaxX, chartMinY+ (*yOffset));
		(*doc) << polyline_axis;

		(*doc) << svg::Text(svg::Point(canvasMinX, chartMinY + (*yOffset) + (*maxMmolGridValue) * (*pixelPerMmol) - fontSize / 2), "[mmol/L]", svg::Color::Black, svg::Font(fontSize, "Verdana"));

		for (unsigned int i = 1; i < (*maxMmolGridValue); i++)
		{
			(*doc) << svg::Text(svg::Point(canvasMinX + 20, chartMinY + (*yOffset) + i * (*pixelPerMmol) - fontSize / 2), std::to_string(i), svg::Color::Black, svg::Font(fontSize, "Verdana"));
			(*doc) << svg::Line(svg::Point(chartMinX, chartMinY + (*yOffset) + i * (*pixelPerMmol)), svg::Point(chartMaxX, chartMinY + (*yOffset) + i * (*pixelPerMmol)), svg::Stroke(1, svg::Color::Grey));
		}

	}
	void SVGExporter::printArrows(svg::Document *doc, double *yOffset)
	{
		svg::Polyline leftArrow(svg::Stroke(2, svg::Color::Black));
		leftArrow << svg::Point(chartMinX - 8, chartMaxY + (*yOffset) - 10) << svg::Point(chartMinX, chartMaxY + (*yOffset) + 5) << svg::Point(chartMinX + 8, chartMaxY + (*yOffset) - 10);
		(*doc) << leftArrow;

		svg::Polyline rightArrow(svg::Stroke(2, svg::Color::Black));
		rightArrow << svg::Point(chartMaxX - 10, chartMinY + (*yOffset) - 8) << svg::Point(chartMaxX + 5, chartMinY + (*yOffset)) << svg::Point(chartMaxX - 10, chartMinY + (*yOffset) + 8);
		(*doc) << rightArrow;
	}
	void SVGExporter::printLegend(svg::Document *doc, double *yOffset)
	{
		double legendStartY = chartMinY + (*yOffset) + (chartHeight) / 2;
		(*doc) << svg::Text(svg::Point(chartMaxX + 25, legendStartY + fontSize), "Namereno", svg::Color::Black, svg::Font(fontSize, "Verdana"));
		(*doc) << svg::Line(svg::Point(chartMaxX + double(legendLineWidth / 2), legendStartY), svg::Point(canvasMaxX - double(legendLineWidth / 2), legendStartY), svg::Stroke(3, svg::Color::Black));

		(*doc) << svg::Text(svg::Point(chartMaxX + 20, legendStartY - fontSize - legendItemMargin), "Detekovano", svg::Color::Black, svg::Font(fontSize, "Verdana"));
		(*doc) << svg::Line(svg::Point(chartMaxX + double(legendLineWidth / 2), legendStartY - fontSize * 2 - legendItemMargin), svg::Point(canvasMaxX - double(legendLineWidth / 2), legendStartY - fontSize * 2 - legendItemMargin), svg::Stroke(4, svg::Color::Red));
		(*doc) << svg::Line(svg::Point(chartMaxX + double(legendLineWidth / 2), legendStartY - fontSize * 2 - legendItemMargin), svg::Point(canvasMaxX - double(legendLineWidth / 2), legendStartY - fontSize * 2 - legendItemMargin), svg::Stroke(2, svg::Color::White));
	}

	void SVGExporter::printPeaks(svg::Document *doc, std::vector<Common::TMeasuredValue*>  *values, std::vector<PeakPeakDetector::Peak> *peaks, const double * columnWidth, const double * pixelPerMmol, double *yOffset)
	{
		for (int i = 0; i < (*peaks).size(); i++)
		{
			svg::Polyline peakLineBackground(svg::Stroke(4, svg::Color::Red));
			svg::Polyline peakLine(svg::Stroke(2, svg::Color::White));


			for (unsigned int j = (*peaks).at(i).startIndex; j < (*peaks).at(i).endIndex; j++)
			{
				peakLineBackground << svg::Point(chartMinX + j*(*columnWidth), (*pixelPerMmol) * (*values).at(j)->ist + chartMinY+ (*yOffset)), 10, svg::Fill(svg::Color::Red);
				peakLine << svg::Point(chartMinX + j*(*columnWidth), (*pixelPerMmol) * (*values).at(j)->ist + chartMinY+ (*yOffset)), 10, svg::Fill(svg::Color::Red);
			}
			(*doc) << peakLineBackground;
			(*doc) << peakLine;
		}
	}

	void SVGExporter::printData(svg::Document *doc, std::vector<Common::TMeasuredValue*>  *values, const double * columnWidth, const double *pixelPerMol, double *yOffset)
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

	void SVGExporter::printYAxis(svg::Document *doc, std::vector<Common::TMeasuredValue*>  *values, double *yOffset)
	{
		if ((*values).size() > 0)
		{
				//pocet minut za den
				unsigned int minutesPerDay = 1440;
				//Rozdil posledniho a prvni v minutach
				unsigned int dataMinutes = ((*values).at((*values).size() - 1)->measureDate - (*values).at(0)->measureDate)* minutesPerDay;

				unsigned int frequency = dataMinutes/25; // kazdych 90 minut bude cara
				double pixelPerMinute = chartWidth / dataMinutes;

				//posunuti popisku doleva
				unsigned int textShiftLeft = 15;
				//posunuti popisku dolu;
				unsigned int textShiftDown = 25;

				double currentTime = (*values).at(0)->measureDate*minutesPerDay;
				std::string minutesStr();
				int hours;
				int minutes;
				for (unsigned int i = 0; i < dataMinutes; i+=frequency)
				{

						(*doc) << svg::Line(svg::Point(chartMinX + (i*pixelPerMinute), chartMinY+ (*yOffset)), svg::Point(chartMinX + (i*pixelPerMinute), chartMaxY+ (*yOffset)), svg::Stroke(1, svg::Color::Grey));
						//TODO: Doplnit nuly
						hours = ((int)(currentTime / 60))%24;
						minutes = (((int)currentTime) % 60);

						(*doc) << svg::Text(svg::Point(chartMinX + (i*pixelPerMinute) - textShiftLeft, chartMinY + (*yOffset) - textShiftDown), std::to_string(hours) + ":" + ((minutes > 9) ? std::to_string(minutes) : std::string("0") + (std::to_string(minutes))), svg::Color::Black, svg::Font((int)(fontSize*.9), "Verdana"));
						currentTime += frequency;
				}
		}
		
	}



	std::string SVGExporter::getFileName(int * segmentId, std::string path)
	{
		std::string exportFileName(path);
		std::string name("my_svg");
		std::string extension(".svg");
		std::string segmentName = std::to_string(*segmentId);
		name = exportFileName + name + segmentName + extension;
		return name;
	}

}
