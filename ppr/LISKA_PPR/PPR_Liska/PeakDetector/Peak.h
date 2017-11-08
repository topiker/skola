#pragma once

#ifdef  PEAKDETECTORDLL_EXPORTS 
/*Enabled as "export" while compiling the dll project*/
#define DLLEXPORT __declspec(dllexport)  
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define DLLEXPORT __declspec(dllimport)  
#endif
class DLLEXPORT Peak
{
public:
	Peak(unsigned int startIndex, unsigned int endIndex);
	~Peak();
private:
	unsigned int startIndex;
	unsigned int endIndex;
};

