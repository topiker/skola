#pragma once
#include <vector>
#include "TMeasuredValue.h"
#include "FromTo.h"
#include "Segment.h"


namespace DataLoader {
	/// <summary>
	/// Trida slouzi pro nacteni dat
	/// </summary>
	class DataLoader
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
		/// Slouzi k nacteni vsech dat naraz
		/// </summary>
		/// <param name="data"></param>
		/// <returns>Pocet nactenych dat</returns>
		size_t loadData(std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data);
		/// <summary>
		/// Rozdeli data po dnech
		/// </summary>
		/// <param name="segmentDays"></param>
		/// <param name="data"></param>
		void splitIntoDays(std::unique_ptr<Common::SegmentDays>& segmentDays,const std::unique_ptr<std::vector<std::unique_ptr<Common::TMeasuredValue>>>& data);
		//void freeData(std::vector<Common::TMeasuredValue *> *data);
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
		std::vector<FromTo> getDaysIndexes(const std::unique_ptr<std::vector<std::unique_ptr<Common::TMeasuredValue>>>& data);
		/// <summary>
		/// Ve vstupnich datech nahradi null ist hodnoty nejlbizsi sousedni
		/// </summary>
		/// <param name="data"></param>
		void smoothNullValues(std::unique_ptr<std::vector<std::unique_ptr<Common::TMeasuredValue>>>& data);
		/// <summary>
		/// Nacte data pro konkretni segment
		/// </summary>
		/// <param name="result"></param>
		/// <param name="segmentId">id pozadovaneho segmentu</param>
		void loadDataBySegment(std::unique_ptr<Common::Segment>& result, int * segmentId);
	};

}

