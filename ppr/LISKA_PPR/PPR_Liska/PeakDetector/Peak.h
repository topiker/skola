#pragma once

#ifdef PEAKDETECTORDLL_EXPORTS 
/*Enabled as "export" while compiling the dll project*/
#define PEAKDLLEXPORT __declspec(dllexport)  
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define PEAKDLLEXPORT __declspec(dllimport) 
#endif


namespace PeakPeakDetector 
{
	class PEAKDLLEXPORT Peak
	{
	public:
		Peak(unsigned int startIndex, unsigned int endIndex);
		unsigned int startIndex;
		unsigned int endIndex;
		//unsigned int getStartIndex();
		//unsigned int getEndIndex();
	private:
		
	};
}
