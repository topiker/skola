#pragma once

namespace SVGExporter {
	//Sirka platna
	const double width = 1500;
	//Vyska platna
	const double height = 500;
	//Odsazeni vykresleni obrazku
	const double canvasMargin = 10;
	//Sirka mezery mezy grafem a okrajem -> osa ypsilon
	const double axisWidth = 80;
	//Vyska mezery mezy grafem a okrajem -> osa x
	const double axisHeight = 50;
	//mezera mezi koncem grafu a hranou canvasu
	const double chartMargin = 10;
	const double fontSize = 12;

	const double legendItemMargin = 15;

	const unsigned int textShiftLeft = 15;
	const unsigned int textShiftDown = 25;



#pragma region legend
	//Sirka legendy
	const double legendWidth = 100;
	//Sirka cary legendy
	const double legendLineWidth = 80;
#pragma endregion

#pragma region CanvasMinMax
	//Minimalni x-ova souradnice platna -> odtud se kresli
	const double canvasMinX = canvasMargin;
	//Maximalni x-ova souradnice platna -> do sem se kresli
	const double canvasMaxX = width - canvasMargin;
	//Minimalni y-ova souradnice platna -> odtud se kresli
	const double canvasMinY = canvasMargin;
	//Maximalni y-ova souradnice platna -> do sem se kresli
	const double canvasMaxY = height - canvasMargin;
#pragma endregion


	const double chartMinX = canvasMinX + axisWidth;
	//Koncova pozice grafu -> x
	const double chartMaxX = canvasMaxX - chartMargin - legendWidth;
	//Pocatecni pozice grafu -> y
	const double chartMinY = canvasMinY + axisHeight;
	//Koncova pozice grafu -> y
	const double chartMaxY = canvasMaxY - chartMargin;
	//Sirka grafu
	const double chartWidth = chartMaxX - chartMinX;
	//Vyska grafu
	const double chartHeight = chartMaxY - chartMinY;
}