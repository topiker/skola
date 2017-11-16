#include "SVGExporter.h"
#include "SVGExporterConstants.h"
#include "DataHelper.h"

namespace SVGExporter {

	SVGExporter::SVGExporter() 
	{
	}

	void  SVGExporter::exportToSvg(std::string path, std::vector<Common::TMeasuredValue*>  *values, int * segmentId, std::vector<PeakPeakDetector::Peak> *peaks)
	{
		double scale = 1.5;
		svg::Dimensions dimensions(width, height);
		svg::Document doc(getFileName(segmentId,path), svg::Layout(dimensions, svg::Layout::BottomLeft));
		/*Zjisteni nejvetsi a nejmensi ist hodnoty v datech*/
		double maxMmolValue = DataHelper::getMaxIstValue(values);

		double maxMmolGridValue = ((double)(int)maxMmolValue) + 1;
		
		//Vypocet hodnoty na pixel
		double pixelPerMmol = chartHeight / maxMmolGridValue;
		//Vypocet sirky sloupce, tak aby se nam vesly do obrazku
		double columnWidth = (chartWidth) / (*values).size();

		this->printData(&doc, values, &columnWidth, &pixelPerMmol);
		this->printLegend(&doc);
		this->printPeaks(&doc, values,peaks, &columnWidth, &pixelPerMmol);
		this->printXAxis(&doc,&pixelPerMmol,&maxMmolGridValue);
		this->printYAxis(&doc, values);
		this->printArrows(&doc);



		//Vykresleni gridu 


		////vykresleni vertikaliniho gridu
		
		doc.save();
	}

	void SVGExporter::printXAxis(svg::Document *doc, const double * pixelPerMmol, const double *maxMmolGridValue)
	{
		svg::Polyline polyline_axis(svg::Stroke(2, svg::Color::Black));
		polyline_axis << svg::Point(chartMinX, chartMaxY) << svg::Point(chartMinX, chartMinY) << svg::Point(chartMaxX, chartMinY);
		(*doc) << polyline_axis;

		(*doc) << svg::Text(svg::Point(canvasMinX, chartMinY + (*maxMmolGridValue) * (*pixelPerMmol) - fontSize / 2), "[mmol/L]", svg::Color::Black, svg::Font(fontSize, "Verdana"));

		for (unsigned int i = 1; i < (*maxMmolGridValue); i++)
		{
			(*doc) << svg::Text(svg::Point(canvasMinX + 20, chartMinY + i * (*pixelPerMmol) - fontSize / 2), std::to_string(i), svg::Color::Black, svg::Font(fontSize, "Verdana"));
			(*doc) << svg::Line(svg::Point(chartMinX, chartMinY + i * (*pixelPerMmol)), svg::Point(chartMaxX, chartMinY + i * (*pixelPerMmol)), svg::Stroke(1, svg::Color::Grey));
		}

	}
	void SVGExporter::printArrows(svg::Document *doc)
	{
		svg::Polyline leftArrow(svg::Stroke(2, svg::Color::Black));
		leftArrow << svg::Point(chartMinX - 8, chartMaxY - 10) << svg::Point(chartMinX, chartMaxY + 5) << svg::Point(chartMinX + 8, chartMaxY - 10);
		(*doc) << leftArrow;

		svg::Polyline rightArrow(svg::Stroke(2, svg::Color::Black));
		rightArrow << svg::Point(chartMaxX - 10, chartMinY - 8) << svg::Point(chartMaxX + 5, chartMinY) << svg::Point(chartMaxX - 10, chartMinY + 8);
		(*doc) << rightArrow;
	}
	void SVGExporter::printLegend(svg::Document *doc)
	{
		double legendStartY = chartMinY + (chartHeight) / 2;
		(*doc) << svg::Text(svg::Point(chartMaxX + 25, legendStartY + fontSize), "Namereno", svg::Color::Black, svg::Font(fontSize, "Verdana"));
		(*doc) << svg::Line(svg::Point(chartMaxX + double(legendLineWidth / 2), legendStartY), svg::Point(canvasMaxX - double(legendLineWidth / 2), legendStartY), svg::Stroke(3, svg::Color::Black));

		(*doc) << svg::Text(svg::Point(chartMaxX + 20, legendStartY - fontSize - legendItemMargin), "Detekovano", svg::Color::Black, svg::Font(fontSize, "Verdana"));
		(*doc) << svg::Line(svg::Point(chartMaxX + double(legendLineWidth / 2), legendStartY - fontSize * 2 - legendItemMargin), svg::Point(canvasMaxX - double(legendLineWidth / 2), legendStartY - fontSize * 2 - legendItemMargin), svg::Stroke(4, svg::Color::Red));
		(*doc) << svg::Line(svg::Point(chartMaxX + double(legendLineWidth / 2), legendStartY - fontSize * 2 - legendItemMargin), svg::Point(canvasMaxX - double(legendLineWidth / 2), legendStartY - fontSize * 2 - legendItemMargin), svg::Stroke(2, svg::Color::White));
	}

	void SVGExporter::printPeaks(svg::Document *doc, std::vector<Common::TMeasuredValue*>  *values, std::vector<PeakPeakDetector::Peak> *peaks, const double * columnWidth, const double * pixelPerMmol)
	{
		for (int i = 0; i < (*peaks).size(); i++)
		{
			svg::Polyline peakLineBackground(svg::Stroke(4, svg::Color::Red));
			svg::Polyline peakLine(svg::Stroke(2, svg::Color::White));


			for (unsigned int j = (*peaks).at(i).startIndex; j < (*peaks).at(i).endIndex; j++)
			{
				peakLineBackground << svg::Point(chartMinX + j*(*columnWidth), (*pixelPerMmol) * (*values).at(j)->ist + chartMinY), 10, svg::Fill(svg::Color::Red);
				peakLine << svg::Point(chartMinX + j*(*columnWidth), (*pixelPerMmol) * (*values).at(j)->ist + chartMinY), 10, svg::Fill(svg::Color::Red);
			}
			(*doc) << peakLineBackground;
			(*doc) << peakLine;
		}
	}

	void SVGExporter::printData(svg::Document *doc, std::vector<Common::TMeasuredValue*>  *values, const double * columnWidth, const double *pixelPerMol)
	{
		svg::Polyline polyline_chart(svg::Stroke(2, svg::Color::Black));
		//svg::Polyline polyline_chart_mean(svg::Stroke(1, svg::Color::Blue));

		for (unsigned i = 0; i < (*values).size(); i++)
		{
			polyline_chart << svg::Point(chartMinX + i*(*columnWidth), (*pixelPerMol) * (*values).at(i)->ist + chartMinY);
			//polyline_chart_mean << svg::Point(chartMinX + i*columnWidth, PixelPerMmol * (*values).at(i)->smoothedValue + chartMinY);
		}
		(*doc) << polyline_chart;
	}

	void SVGExporter::printYAxis(svg::Document *doc, std::vector<Common::TMeasuredValue*>  *values)
	{
		//if ((*values).size() > 0)
		//{


		//	//std::cout<< (*values).at(0)->measureDate)
		//	//pocet minut za den
		//	unsigned int minutesPerDay = 1440;
		//	//doba mereni, tj jednoho segmentu, v minutách!

		//	unsigned int minutes = ((*values).size() - 1) * 5;
		//	//ziskani mantisy
		//	//jak casto chceme vykreslovat grid
		//	unsigned int frequency = 30; // kazdych 90 minut bude cara
		//	double PixelPerMinute = chartWidth / minutes;

		//	//posunuti popisku doleva
		//	unsigned int textShiftLeft = 15;
		//	//posunuti popisku dolu;
		//	unsigned int textShiftDown = 25;

		//	for (unsigned int i = 0; i < minutes; i++)
		//	{
		//		unsigned int currentTime = i + startTime;
		//		if (currentTime % frequency == 0) {

		//			//kvuli preteceni do dalsiho dne provadime operaci modulo
		//			currentTime = currentTime % minutesPerDay;
		//			(*doc) << svg::Line(svg::Point(chartMinX + (i*PixelPerMinute), chartMinY), svg::Point(chartMinX + (i*PixelPerMinute), chartMaxY), svg::Stroke(1, svg::Color::Grey));
		//			//TODO: Doplnit nuly
		//			(*doc) << svg::Text(svg::Point(chartMinX + (i*PixelPerMinute) - textShiftLeft, chartMinY - textShiftDown), std::to_string((int)(currentTime / 60)) + ":" + std::to_string((int)(currentTime % 60)), svg::Color::Black, svg::Font((int)(fontSize*.9), "Verdana"));
		//		}
		//	}
		//}
		
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
