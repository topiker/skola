#include "DataLoader.h"
#include "sqlite3.h"
#include <iostream>
#include "SqlQueries.h"
#include "DatabaseConnector.h"
// DataLoader.cpp : Defines the entry point for the console application.
//

namespace DataLoader {

	DataLoader::DataLoader(char * dbPath)
	{
		this->dbPath = dbPath;
	}

	int DataLoader::getSegmentIds(std::vector<int> *segmentIds)
	{
		std::cout << "Ziskavam id segmentu" << std::endl;
		DatabaseConnector connector = DatabaseConnector::DatabaseConnector(&(this->dbPath));

		connector.executeCommand(segmentsQuery, segmentsQueryCallback, (void*)segmentIds);
		
		return 0;
	}

	size_t DataLoader::loadData(std::vector<TMeasuredValue*> *data, int * segmentId)
	{
		std::string sqlStr = "SELECT (julianday(measuredat) - 2415019.5) AS 'measuredat',id, segmentid, ist FROM measuredvalue WHERE segmentId=" + std::to_string(*segmentId) + " ORDER BY measuredat";
		const char * sql = sqlStr.c_str();
		DatabaseConnector connector = DatabaseConnector::DatabaseConnector(&(this->dbPath));
		connector.executeCommand(sql, dataQueryCallback, (void*)data);

		return (*data).size();
	}


	DataLoader::~DataLoader()
	{
	}

}