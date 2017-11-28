#pragma once

#include <iostream>
#include <string>


namespace Parser
{
	/// <summary>
	/// Enum vstupnich parametru
	/// </summary>
	enum ARGUMENTSENUM
	{
		DB,
		METHOD,
		WINDOW,
		EXPORTPATH,
		GRAPHPERDAY,
		NOTMATCHED,
	};
	/// <summary>
	/// Trida slouzi pro vstup z konzole
	/// </summary>
	class InputParser
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="paramsCount">Pocet vstupnich parametru</param>
		/// <param name="params">parametry v poli</param>
		InputParser(int *paramsCount, char** params);
		~InputParser();
		/// <summary>
		/// Test, zda jsou vstupni parametry v poradku
		/// </summary>
		/// <returns></returns>
		bool areParamstOk();
		/// <summary>
		/// Vraci cestu k databazi
		/// </summary>
		/// <returns></returns>
		char * getDbPath();
		/// <summary>
		/// Vraci cestu k vystupni slozce pro export svg
		/// </summary>
		/// <returns></returns>
		char * getExportPath();
		/// <summary>
		/// Zda ma vypocet bezet na akceleratoru
		/// </summary>
		/// <returns></returns>
		bool isGpu();
		/// <summary>
		/// Zda ma vypocet bezet paralelne
		/// </summary>
		/// <returns></returns>
		bool isParallel();
		/// <summary>
		/// Zda ma paralelni vypocet bezet i v ramci jednotlivych dnu
		/// </summary>
		/// <returns></returns>
		bool isParalelismPerDay();
		/// <summary>
		/// Zda se ma provest svg export
		/// </summary>
		/// <returns></returns>
		bool isDoExport();
		/// <summary>
		/// Zda se ma vykreslit pro kazdy den extra graf
		/// </summary>
		/// <returns></returns>
		bool isGraphPerDay();
		/// <summary>
		/// Velikost okna pro detekci
		/// </summary>
		/// <returns></returns>
		int getWindowSize();
		/// <summary>
		/// Vypis info zpravy o vstupnich parametrech
		/// </summary>
		void printInfo();
		/// <summary>
		/// Vypis napovedy
		/// </summary>
		void printHelp();

	private:
		/// <summary>
		/// Priznak, zda se ma vypocet provest na akceleratoru
		/// </summary>
		bool useGPU;
		/// <summary>
		/// Priznak, zda se na provadet export do svg
		/// </summary>
		bool doExport;
		/// <summary>
		/// Priznak, zda se ma vypocet provadet paralelne
		/// </summary>
		bool parallel;
		/// <summary>
		/// Priznak, zda ma kazdy den mit extra graf
		/// </summary>
		bool graphPerDay;
		/// <summary>
		/// Velikost okna pro detekci
		/// </summary>
		int windowSize;
		/// <summary>
		/// Priznak, zda se ma paralelizovat i vypocet v ramci dne
		/// </summary>
		bool paralelismPerDay;
		/// <summary>
		/// Cesta k db
		/// </summary>
		char * dbPath;
		/// <summary>
		/// Cesta k export slozce
		/// </summary>
		char * exportPath;
		/// <summary>
		/// Priznak, zda jsou parametry spravne
		/// </summary>
		bool paramsOk;
		/// <summary>
		/// Ze vstupniho retezce vrati ciselnik parametru, ktery se ma zpracovat
		/// </summary>
		/// <param name="paramName"></param>
		/// <returns></returns>
		ARGUMENTSENUM getParamEnumFromName(const std::string *paramName);
		/// <summary>
		/// Ze vstupnich parametru nastavi hodnoty atributum tridy
		/// </summary>
		/// <param name="paramsCount"></param>
		/// <param name="params"></param>
		void setParams(int * paramsCount, char** params);

	};

	
}