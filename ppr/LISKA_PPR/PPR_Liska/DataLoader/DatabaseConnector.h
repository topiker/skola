#pragma once
#include <iostream>
#include "sqlite3.h"

class DatabaseConnector
{
public:
	DatabaseConnector(const char **dbPath);
	~DatabaseConnector();
	bool executeCommand(const char * query, int callback(void *pArg, int argc, char **argv, char **columnNames), void *data);
private:
	bool connectionOk;
	sqlite3 *db;
};

