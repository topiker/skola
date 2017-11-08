#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#ifdef  INPUTPARSERDLL_EXPORTS 
/*Enabled as "export" while compiling the dll project*/
#define DLLEXPORT __declspec(dllexport)  
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define DLLEXPORT __declspec(dllimport)  
#endif

#include <iostream>
#include <string>


namespace Parser
{
	enum ARGUMENTSENUM
	{
		DB,
		GPU,
		OUTPUT,
		NOTMATCHED,
	};

	class DLLEXPORT InputParser
	{
	public:
		InputParser(int *paramsCount, char** params);
		bool areParamstOk();
		const char * getDbPath();
		const char * getExportPath();

	private:
		bool useGPU;
		const char* dbPath;
		const char* exportPath;
		void setParams(int * paramsCount, char** params);
		bool paramsOk;
		ARGUMENTSENUM getParamEnumFromName(const std::string *paramName);
	};

	
}

#endif INPUTPARSER_H