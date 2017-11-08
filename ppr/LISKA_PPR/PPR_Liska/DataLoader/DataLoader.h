#pragma once
#include <vector>
#include "TMeasuredValue.h"

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
		DataLoader(std::string dbPath);
		int getSegmentIds(std::vector<int> *getSegmentIds);
		void loadData(std::vector<TMeasuredValue*> *data, int * segmentId);
		
		~DataLoader();
	private:
		std::string dbPath;
	};

}
