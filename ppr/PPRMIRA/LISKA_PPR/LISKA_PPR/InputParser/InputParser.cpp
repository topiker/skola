#include "InputParser.h"
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
		bool graphPerDayInput = false;

		//Predpokladam, ze budu ocekat parametry ve formatu -nazevParametru hodnota
		//if ((*paramsCount) == 9)
		//{
			for (unsigned int i = 1; i < unsigned(*paramsCount); i = i + 2)
			{
				paramName = (params)[i];
				paramValue = (params)[i + 1];
				ARGUMENTSENUM param = this->getParamEnumFromName(&paramName);
				switch (param)
				{
				case Parser::DB:
					this->dbPath = new char[paramValue.size() + 1];
					strcpy_s((this->dbPath), paramValue.size() + 1, paramValue.c_str());
					dbPathInput = true;
					break;
				case Parser::METHOD:
					if (paramValue == "1")
					{
						//Seriove
						this->useGPU = false;
						this->parallel = false;
						this->paralelismPerDay = false;
					}
					else if (paramValue == "2")
					{
						//Paralelne cely segment
						this->useGPU = false;
						this->parallel = true;
						this->paralelismPerDay = false;
					}
					else if (paramValue == "3")
					{
						//Paralelne segment a jeste paralelismus v ramci dni
						this->useGPU = false;
						this->parallel = true;
						this->paralelismPerDay = true;
					}
					else if (paramValue == "4")
					{
						//Accelerator
						this->useGPU = true;
						this->parallel = false;
						this->paralelismPerDay = false;
					}
					else
					{
						//Kdyz nic, tak seriove
						this->useGPU = false;
						this->parallel = false;
						this->paralelismPerDay = false;
					}
					paralelismInput = true;
					break;
				case Parser::EXPORTPATH:
					if (paramValue == ":NOEXPORT:")
					{
						this->doExport = false;
					}
					else 
					{
						this->doExport = true;
						this->exportPath = new char[paramValue.size() + 1];
						strcpy_s((this->exportPath), paramValue.size() + 1, paramValue.c_str());
					}
					outputInput = true;
					break;
				case Parser::WINDOW:
					this->windowSize = atoi(paramValue.c_str());
					if (this->windowSize > 0)
					{
						windowSizeInput = true;
					}
					break;
				case Parser::GRAPHPERDAY:
					if (paramValue == "1")
					{
						this->graphPerDay = true;
					}
					else 
					{
						this->graphPerDay = false;
					}
					graphPerDayInput = true;
					break;
				case Parser::NOTMATCHED:
					break;
				default:
					break;
				}
			}

		if (paralelismInput && dbPathInput && windowSizeInput)
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
		if ((*paramName) == "-method") return Parser::METHOD;
		if ((*paramName) == "-exportPath") return Parser::EXPORTPATH;
		if ((*paramName) == "-window") return Parser::WINDOW;
		if ((*paramName) == "-graphPerDay") return Parser::GRAPHPERDAY;


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

	bool InputParser::isParalelismPerDay()
	{
		return this->paralelismPerDay;
	}

	bool InputParser::isDoExport()
	{
		return this->doExport;
	}

	bool InputParser::isGraphPerDay()
	{
		return this->graphPerDay;
	}


	void InputParser::printInfo()
	{
		std::cout << "DETEKTOR ZMENY KONCENTRACE GLUKOZY" << std::endl;
		std::cout << "----------------------------------" << std::endl;
		std::cout << "Program byl spusten s nasledujicimi parametry" << std::endl;
		std::string typBehu = this->useGPU ? "Akcelerator" : (this->parallel == false ? "Seriove" : (this->paralelismPerDay ? "Paralelne na urovni segmentu a dni" : "Paralelne na urovni segmentu"));
		std::cout << "Typ behu: " << typBehu << std::endl;
		std::cout << "Velikost okna: " << this->windowSize << std::endl;
		std::cout << "Cesta k databazi: " << this->dbPath << std::endl;
		std::string exportText = (this->doExport == false ? "Neexportovat" : this->exportPath);
		std::cout << "Adresar pro export: " << exportText << std::endl;
		if (this->doExport)
		{
			std::cout << "Grafy: " << (this->graphPerDay ? " Segment po dnech" : "Segment v jednom grafu") << std::endl;
		}


	}

	void InputParser::printHelp() 
	{
		std::cout << "Program ocekava nasledujici parametry" << std::endl;
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "-db [cesta k databazi]" << std::endl;
		std::cout << "-method [identifikator zvolene metody: 1 - seriove, 2 - paralelne na urovni semgnetu, 3 - paralelne na zaklade dnu, 4 - akcelerator]" << std::endl;
		std::cout << "-exportPath [kam se ma exportovat svg]" << std::endl;
		std::cout << "-window [velikost detekcniho okna]" << std::endl;
		std::cout << "-graphPerDay [identifikator zvoleneho grafu: 0 - Segment v jednom grafu, 1 - Co den, to graf]" << std::endl;
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "Napr -db direcnet.sqlite -method 1 -exportPath /ExportFolder -window 24" << std::endl;
		std::cout << "Napr -db direcnet.sqlite -method 3 -exportPath :NOEXPORT: -window 24" << std::endl;





	}


}
