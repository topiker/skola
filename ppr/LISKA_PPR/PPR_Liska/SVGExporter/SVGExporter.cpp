#include "SVGExporter.h"
#include <vector>
#include "simple_svg_1.0.0.hpp"

namespace SVGExporter {

	void  SVGExporter::exportToSvg(std::string path, std::vector<TMeasuredValue*>  *values, int * segmentId)
	{
		double scale = 1.5;
		//Sirka platna
		double width = 1000;
		//Vyska platna
		double height = 500;
		//Odsazeni vykresleni obrazku
		double canvasMargin = 10;
		#pragma region CanvasMinMax
		//Minimalni x-ova souradnice platna -> odtud se kresli
		double canvasMinX = canvasMargin;
		//Maximalni x-ova souradnice platna -> do sem se kresli
		double canvasMaxX = width - canvasMargin;
		//Minimalni y-ova souradnice platna -> odtud se kresli
		double canvasMinY = canvasMargin;
		//Maximalni y-ova souradnice platna -> do sem se kresli
		double canvasMaxY = height - canvasMargin;
		#pragma endregion

		//Sirka mezery mezy grafem a okrajem -> osa ypsilon
		double axisWidth = 80;
		//Vyska mezery mezy grafem a okrajem -> osa x
		double axisHeight = 50;

		//mezera mezi koncem grafu a hranou canvasu
		double chartMargin = 10;
		//Pocatecni pozice grafu ->x
		double chartMinX = canvasMinX + axisWidth;
		//Koncova pozice grafu -> x
		double chartMaxX = canvasMaxX - chartMargin;
		//Pocatecni pozice grafu -> y
		double chartMinY = canvasMinY + axisHeight;
		//Koncova pozice grafu -> y
		double chartMaxY = canvasMaxY - chartMargin;
		//Sirka grafu
		double chartWidth = chartMaxX - chartMinX;
		//Vyska grafu
		double chartHeight = chartMaxY - chartMinY;



		svg::Dimensions dimensions(width, height);
		std::string exportFileName(path);
		std::string name("my_svg");
		std::string extension(".svg");
		std::string segmentName = std::to_string(*segmentId);

		name = exportFileName + name + segmentName + extension;
		svg::Document doc(name, svg::Layout(dimensions, svg::Layout::BottomLeft));

		/*Vykresleni ramecku kolem obrazku*/
		//Polygon border(Stroke(2, Color::Black));
		//border
		//	<< Point(canvasMinX, canvasMinY) << Point(canvasMaxX, canvasMinY)
		//	<< Point(canvasMaxX, canvasMaxY) << Point(canvasMinX, canvasMaxY);
		//doc << border;

		/*Zjisteni nejvetsi a nejmensi ist hodnoty v datech*/
		double maxMmolValue = -DBL_MAX;
		for each (TMeasuredValue *item in *values)
		{
			if ((item)->ist > maxMmolValue)
			{
				maxMmolValue = item->ist;
			}
		}

		double overleapTop = 0;
		double maxMmolGridValue = ((double)(int)maxMmolValue) + 1 + overleapTop;


		//Vypocet hodnoty na pixel
		double PixelPerMmol = chartHeight / maxMmolGridValue;
		//Vypocet sirky sloupce, tak aby se nam vesly do obrazku
		double columnWidth = (chartWidth) / (*values).size();

		svg::Polyline polyline_chart(svg::Stroke(2, svg::Color::Black));

		bool drawPeak = false;
		for (unsigned i = 0; i < (*values).size(); i++)
		{
			//Test, zda se jedna o peak, pokud ano, vykreslim bod
			//drawPeak = false;
			//for (unsigned k = 0; k < (*peaks).size(); k++)
			//{
			//	if ((*peaks).at(k) == i)
			//	{
			//		drawPeak = true;
			//		break;
			//	}
			//}
			polyline_chart << svg::Point(chartMinX + i*columnWidth, PixelPerMmol * (*values).at(i)->ist + chartMinY);
			////Test, zda se jedna o peak, pokud ano, vykreslim bod
			//if (drawPeak)
			//{
			//	doc << svg::Circle(svg::Point(chartMinX + i*columnWidth, PixelPerMmol * (*values).at(i)->ist + chartMinY), 10 * scale, svg::Fill(svg::Color::Red), Stroke(1 * scale, svg::Color::Red));
			//}
		}
		doc << polyline_chart;


		//Vykresleni os
		svg::Polyline polyline_axis(svg::Stroke(2, svg::Color::Black));
		polyline_axis << svg::Point(chartMinX, chartMaxY) << svg::Point(chartMinX, chartMinY) << svg::Point(chartMaxX, chartMinY);
		doc << polyline_axis;

		svg::Polyline leftArrow(svg::Stroke(2, svg::Color::Black));
		leftArrow << svg::Point(chartMinX - 8, chartMaxY - 10) << svg::Point(chartMinX, chartMaxY + 5) << svg::Point(chartMinX + 8, chartMaxY - 10);
		doc << leftArrow;

		svg::Polyline rightArrow(svg::Stroke(2, svg::Color::Black));
		rightArrow << svg::Point(chartMaxX - 10, chartMinY - 8) << svg::Point(chartMaxX + 5, chartMinY) << svg::Point(chartMaxX - 10, chartMinY + 8);
		doc << rightArrow;
		//Vykresleni gridu 
		// kresli horizontalni grid
		double fontSize = 12;
		doc << svg::Text(svg::Point(canvasMinX, chartMinY + maxMmolGridValue * PixelPerMmol - fontSize / 2), "[mmol/L]", svg::Color::Black, svg::Font(fontSize, "Verdana"));

		for (unsigned int i = 1; i < maxMmolGridValue; i++)
		{
			doc << svg::Text(svg::Point(canvasMinX + 20, chartMinY + i * PixelPerMmol - fontSize / 2), std::to_string(i), svg::Color::Black, svg::Font(fontSize, "Verdana"));
			doc << svg::Line(svg::Point(chartMinX, chartMinY + i * PixelPerMmol), svg::Point(chartMaxX, chartMinY + i * PixelPerMmol), svg::Stroke(1 * scale / 2, svg::Color::Black));
		}

		//vykresleni vertikaliniho gridu
		//pocet minut za den
		unsigned int minutesPerDay = 1440;
		//doba mereni, tj jednoho segmentu, v minutách!
		unsigned int minutes = (unsigned int)(((*values).at((*values).size() - 1)->measureDate - (*values).at(0)->measureDate) * minutesPerDay);
		//ziskani mantisy
		double mantisa = (*values).at(0)->measureDate - (int)((*values).at(0)->measureDate);
		//doba v minutach, kdy zacalo mereni (v ramci jednoho dne)
		unsigned int startTime = (int)(mantisa * minutesPerDay);

		//jak casto chceme vykreslovat grid
		unsigned int frequency = 60; // kazdych 90 minut bude cara
		double PixelPerMinute = chartWidth / minutes;

		//posunuti popisku doleva
		unsigned int textShiftLeft = 15;
		//posunuti popisku dolu;
		unsigned int textShiftDown = 25;
		int internalCount = 0;
		for (unsigned int i = 1; i < minutes; i++)
		{
			unsigned int currentTime = i + startTime;
			if (currentTime % frequency == 0) {

				//kvuli preteceni do dalsiho dne provadime operaci modulo
				currentTime = currentTime % minutesPerDay;
				if (internalCount != 0)
				{
					doc << svg::Line(svg::Point(chartMinX + (i*PixelPerMinute), chartMinY), svg::Point(chartMinX + (i*PixelPerMinute), chartMaxY), svg::Stroke(1 * scale / 2, svg::Color::Black));
				}
				internalCount++;
				doc << svg::Text(svg::Point(chartMinX + (i*PixelPerMinute) - textShiftLeft, chartMinY - textShiftDown), std::to_string((int)(currentTime / 60)) + ":" + std::to_string((int)(currentTime % 60)), svg::Color::Black, svg::Font((int)(fontSize*.9), "Verdana"));
			}
		}
		doc.save();
	}

}
