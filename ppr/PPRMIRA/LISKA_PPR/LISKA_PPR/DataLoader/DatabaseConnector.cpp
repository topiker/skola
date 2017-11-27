#include "DatabaseConnector.h"
#include "sqlite3.h"

namespace DataLoader {

	DatabaseConnector::DatabaseConnector(const char **dbPath)
	{
		int rc;

		rc = sqlite3_open(*dbPath, &(this->db));
		if (rc)
		{
			std::cerr << "Problem s otevrenim databaze: " << sqlite3_errmsg(this->db) << std::endl << std::endl;
			connectionOk = false;
		}
		else
		{
			//std::cout << "Spojeni otevreno." << std::endl;
			connectionOk = true;
		}
	}
	bool DatabaseConnector::executeCommand(const char * query, int callback(void *pArg, int argc, char **argv, char **columnNames), void *data)
	{

		int rc = 0;
		char *zErrMsg = 0;
		rc = sqlite3_exec(db, query, callback, data, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
			return false;
		}
		return true;
	}

	DatabaseConnector::~DatabaseConnector()
	{
		//std::cout << "Zaviram spojeni." << std::endl;
		sqlite3_close(db);
	}

}