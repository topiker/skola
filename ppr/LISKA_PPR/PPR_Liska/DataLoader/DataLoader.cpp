#include "DataLoader.h"
#include "sqlite3.h"
#include <iostream>
#include "SqlQueries.h"
// DataLoader.cpp : Defines the entry point for the console application.
//

namespace DataLoader {

	DataLoader::DataLoader(std::string dbPath)
	{
		this->dbPath = dbPath;
	}

	int DataLoader::getSegmentIds(std::vector<int> *segmentIds)
	{
		int rc;
		char *error = 0;

		sqlite3 *db;

		rc = sqlite3_open((this->dbPath.c_str()), &db);

		if (rc)
		{
			std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << std::endl << std::endl;
			sqlite3_close(db);
			return 1;
		}
		else
		{
			std::cout << "Opened directnet.sqlite." << std::endl << std::endl;
		}

		char *zErrMsg = 0;
		rc = sqlite3_exec(db, segmentsQuery, segmentsQueryCallback, (void*)segmentIds, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else
		{
			fprintf(stdout, "Operation done successfully\n");
		}
		sqlite3_close(db);
		return 0;
	}

	void DataLoader::loadData(std::vector<TMeasuredValue*> *data, int * segmentId)
	{

	}


	DataLoader::~DataLoader()
	{
	}

}