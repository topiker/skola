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

	class DATALOADERDLLEXPORT DataLoader
	{
	public:
		DataLoader(char * dbPath);
		int getSegmentIds(std::vector<int> *getSegmentIds);
		size_t loadData(std::vector<Common::TMeasuredValue*> *data, int * segmentId);
		void splitIntoDays(std::vector<Common::SegmentDay> *days, std::vector<Common::TMeasuredValue *> *data);
		
		~DataLoader();
	private:
		const char * dbPath;
		std::vector<FromTo> getDaysIndexes(std::vector<Common::TMeasuredValue*> *data);
	};

}
