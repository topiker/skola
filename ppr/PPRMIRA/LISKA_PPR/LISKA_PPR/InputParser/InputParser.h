#pragma once

#include <iostream>
#include <string>


namespace Parser
{
	enum ARGUMENTSENUM
	{
		DB,
		METHOD,
		WINDOW,
		EXPORTPATH,
		GRAPHPERDAY,
		NOTMATCHED,
	};

	class InputParser
	{
	public:
		InputParser(int *paramsCount, char** params);
		~InputParser();

		bool areParamstOk();
		char * getDbPath();
		char * getExportPath();
		bool isGpu();
		bool isParallel();
		bool isParalelismPerDay();
		bool isDoExport();
		bool isGraphPerDay();
		int getWindowSize();

		void printInfo();
		void printHelp();

	private:
		bool useGPU;
		bool doExport;
		bool parallel;
		bool graphPerDay;
		int windowSize;
		bool paralelismPerDay;
		char * dbPath;
		char * exportPath;
		void setParams(int * paramsCount, char** params);
		bool paramsOk;
		ARGUMENTSENUM getParamEnumFromName(const std::string *paramName);
	};

	
}