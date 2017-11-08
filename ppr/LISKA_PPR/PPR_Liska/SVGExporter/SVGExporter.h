#pragma once
#include "TMeasuredValue.h"
#include <vector>

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
		void exportToSvg(std::string path, std::vector<TMeasuredValue*>  *toExport);
	};

}

