#include "../Headers/InputParser.h"
#include <iostream>

namespace Parser{



	InputParser::InputParser(int *paramsCount, char** params)
	{
		this->setParams(paramsCount, params);
	}

	void InputParser::setParams(int * paramsCount, char** params)
	{
		std::string paramName;
		std::string paramValue;

		//Predpokladam, ze budu ocekat parametry ve formatu -nazevParametru hodnota
		if (((*paramsCount) > 1) && (((*paramsCount) % 2) == 1))
		{
			for (size_t i = 1; i < (*paramsCount); i=i+2)
			{
				paramName = (params)[i];
				paramValue = (params)[i+1];
				ARGUMENTSENUM param = this->getParamEnumFromName(&paramName);
				switch (param)
				{
				case Parser::DB:
					this->dbPath = (paramValue.c_str());
					break;
				case Parser::GPU:
					this->useGPU = paramValue == ("0") ? false : true;
					break;
				case Parser::OUTPUT:
					this->exportPath = (paramValue.c_str());
					break;
				case Parser::NOTMATCHED:
					break;
				default:
					break;
				}
			}
		}
		else 
		{
			this->paramsOk = false;
		}
	}

	ARGUMENTSENUM InputParser::getParamEnumFromName(const std::string *paramName)
	{
		if ((*paramName) == "-db") return Parser::DB;
		if ((*paramName) == "-useGpu") return Parser::GPU;
		if ((*paramName) == "-outputDir") return Parser::OUTPUT;

		return NOTMATCHED;
	}


	bool InputParser::areParamstOk()
	{
		return this->paramsOk;
	}

	const char * InputParser::getDbPath()
	{
		return this->dbPath;
	}

	const char * InputParser::getExportPath()
	{
		return this->exportPath;
	}
}
