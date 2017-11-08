#include "DataLoader.h"
// DataLoader.cpp : Defines the entry point for the console application.
//

namespace DataLoader {

	DataLoader::DataLoader(const char * dbPath)
	{
		this->dbPath = dbPath;
	}

	void DataLoader::getSegmentIds(std::vector<int> *getSegmentIds)
	{

	}

	void DataLoader::loadData(std::vector<TMeasuredValue*> *data, int * segmentId)
	{

	}


	DataLoader::~DataLoader()
	{
	}

}