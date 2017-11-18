#pragma once
#include <vector>
#include "TMeasuredValue.h"
#include "SegmentDay.h"
#include "FromTo.h"

#ifdef  DATALOADERDLL_EXPORTS 
/*Enabled as "export" while compiling the dll project*/
#define DATALOADERDLLEXPORT __declspec(dllexport)  
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define DATALOADERDLLEXPORT __declspec(dllimport)  
#endif
namespace DataLoader {
	/// <summary>
	/// Trida slouzi pro nacteni dat
	/// </summary>
	class DATALOADERDLLEXPORT DataLoader
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="dbPath">Cesta k databazi, odkud se budou data nacitat</param>
		DataLoader(char * dbPath);
		/// <summary>
		/// Metoda nacte id segmentu z databaze
		/// </summary>
		/// <param name="getSegmentIds"></param>
		/// <returns></returns>
		size_t getSegmentIds(std::vector<int> *getSegmentIds);
		/// <summary>
		/// Metoda slouzi k nacteni dat z databaze
		/// </summary>
		/// <param name="data">Kam se data nactou</param>
		/// <param name="segmentId">Id segmentu, jehoz data nas zajimaji</param>
		/// <returns>Pocet nactenych dat</returns>
		size_t loadData(std::vector<Common::TMeasuredValue*> *data, int * segmentId);
		/// <summary>
		/// Rozdeli v stupni data podle jejich datumu na dny
		/// </summary>
		/// <param name="days">Jedna polozka vektoru = 1 den</param>
		/// <param name="data">Data, ze kterych se cte</param>
		void splitIntoDays(std::vector<Common::SegmentDay> *days, std::vector<Common::TMeasuredValue *> *data);
		
		~DataLoader();
	private:
		/// <summary>
		/// Cesta k databazi
		/// </summary>
		const char * dbPath;
		/// <summary>
		/// Vraci index od do pro rozdeleni dat podle dne. Tedy par FromTo vzdy reprezentuje indexi v datech od od pro rozdeleni na den
		/// </summary>
		/// <param name="data"></param>
		/// <returns>Rozsahy indexu jednotlivych dnu v datech</returns>
		std::vector<FromTo> getDaysIndexes(std::vector<Common::TMeasuredValue*> *data);
	};

}
