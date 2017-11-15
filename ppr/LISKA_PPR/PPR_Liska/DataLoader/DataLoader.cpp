#include "DataLoader.h"
#include "sqlite3.h"
#include <iostream>
#include "SqlQueries.h"
// DataLoader.cpp : Defines the entry point for the console application.
//

namespace DataLoader {

	DataLoader::DataLoader(char * dbPath)
	{
		this->dbPath = dbPath;
	}

	int DataLoader::getSegmentIds(std::vector<int> *segmentIds)
	{
		int rc;
		char *error = 0;

		sqlite3 *db;

		rc = sqlite3_open((this->dbPath), &db);

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
			//fprintf(stdout, "Operation done successfully\n");
		}
		sqlite3_close(db);
		return 0;
	}

	int DataLoader::loadData(std::vector<TMeasuredValue*> *data, int * segmentId)
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;

		rc = sqlite3_open(this->dbPath, &db);

		if (rc) {
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			return(1);
		}
		else {
			//fprintf(stderr, "Opened database successfully\n");
		}



		//pridame tam segment id
		std::string sqlStr = "SELECT (julianday(measuredat) - 2415019.5) AS 'measuredat',id, segmentid, ist FROM measuredvalue WHERE segmentId=" + std::to_string(*segmentId) + " ORDER BY measuredat";
		const char * sql = sqlStr.c_str();

		// Execute SQL statement 
		// zpracovani vsech kazdeho ziskaneho segmentu (kazdeho radku z tabulky) probehne ve funkci callback
		rc = sqlite3_exec(db, sql, dataQueryCallback, (void*)data, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			//fprintf(stdout, "DB Select operation done successfully\n");
		}
		sqlite3_close(db);

		return rc;
	}


	DataLoader::~DataLoader()
	{
	}

}