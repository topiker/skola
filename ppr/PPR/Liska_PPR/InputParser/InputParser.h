#pragma once

#include <iostream>
#include <string>


namespace Parser
{
	enum ARGUMENTSENUM
	{
		DB,
		PARALELISM,
		WINDOWSIZE,
		OUTPUT,
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
		int getWindowSize();

	private:
		bool useGPU;
		bool parallel;
		int windowSize;
		char * dbPath;
		char * exportPath;
		void setParams(int * paramsCount, char** params);
		bool paramsOk;
		ARGUMENTSENUM getParamEnumFromName(const std::string *paramName);
	};

	
}