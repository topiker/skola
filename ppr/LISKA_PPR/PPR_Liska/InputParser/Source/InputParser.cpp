#include "../Headers/InputParser.h"
#include <iostream>

namespace Parser {



	InputParser::InputParser(int *paramsCount, char** params)
	{
		this->setParams(paramsCount, params);
	}

	InputParser::~InputParser() 
	{
		if (this->dbPath != nullptr)
		{
			delete(this->dbPath);
			this->dbPath = NULL;
		}
		if (this->exportPath != nullptr)
		{
			delete(this->exportPath);
			this->exportPath = NULL;
		}
	}

	void InputParser::setParams(int * paramsCount, char** params)
	{
		std::string paramName;
		std::string paramValue;
		bool dbPathInput = false;
		bool paralelismInput = false;
		bool outputInput = false;
		bool windowSizeInput = false;

		//Predpokladam, ze budu ocekat parametry ve formatu -nazevParametru hodnota
		if ((*paramsCount) == 9)
		{
			for (unsigned int i = 1; i < unsigned(*paramsCount); i = i + 2)
			{
				paramName = (params)[i];
				paramValue = (params)[i + 1];
				ARGUMENTSENUM param = this->getParamEnumFromName(&paramName);
				//TODO: Vypsat co je spatne
				switch (param)
				{
				case Parser::DB:
					this->dbPath = new char[paramValue.size() + 1];
					strcpy_s((this->dbPath), paramValue.size() + 1, paramValue.c_str());
					dbPathInput = true;
					break;
				case Parser::PARALELISM:
					if (paramValue=="1")
					{
						this->useGPU = false;
						this->parallel = true;
					}
					else if (paramValue=="2")
					{
						this->useGPU = true;
						this->parallel = false;
					}
					else 
					{
						this->useGPU = false;
						this->parallel = false;
					}
					paralelismInput = true;
					break;
				case Parser::OUTPUT:
					this->exportPath = new char[paramValue.size() + 1];
					strcpy_s((this->exportPath), paramValue.size() + 1, paramValue.c_str());
					outputInput = true;
					break;
				case Parser::WINDOWSIZE:
					this->windowSize = atoi(paramValue.c_str());
					if (this->windowSize > 0)
					{
						windowSizeInput = true;
					}
					break;
				case Parser::NOTMATCHED:
					break;
				default:
					break;
				}
			}

		}
		if (outputInput && paralelismInput && dbPathInput && windowSizeInput)
		{
			this->paramsOk = true;
		}
		else
		{
			this->paramsOk = false;
		}

	}

	ARGUMENTSENUM InputParser::getParamEnumFromName(const std::string *paramName)
	{
		if ((*paramName) == "-db") return Parser::DB;
		if ((*paramName) == "-paralelism") return Parser::PARALELISM;
		if ((*paramName) == "-outputDir") return Parser::OUTPUT;
		if ((*paramName) == "-windowSize") return Parser::WINDOWSIZE;

		return NOTMATCHED;
	}


	bool InputParser::areParamstOk()
	{
		return this->paramsOk;
	}

	char * InputParser::getDbPath()
	{
		return this->dbPath;
	}

	char * InputParser::getExportPath()
	{
		return this->exportPath;
	}

	bool InputParser::isGpu()
	{
		return this->useGPU;
	}

	bool InputParser::isParallel()
	{
		return this->parallel;
	}

	int InputParser::getWindowSize()
	{
		return this->windowSize;
	}
}
