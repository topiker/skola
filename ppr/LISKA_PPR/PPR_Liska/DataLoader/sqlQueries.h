#pragma once

#include <iostream>
#include <vector>
#include <string>


const char *segmentsQuery = "SELECT id FROM timesegment ORDER BY id ASC;";
const char *measuredvalueQuery = "SELECT (julianday(measuredat)) AS 'measuredat',id, segmentid, ist FROM measuredvalue WHERE segmentId=%i ORDER BY measuredat;";


static int segmentsQueryCallback(void *data, int argc, char **argv, char **azColName)
{
	std::vector<int> *segments = (std::vector<int>*)data;

	int segmentId;
	int i;
	for (i = 0; i < argc; i++)
	{
		if (strcmp(azColName[i], "id") == 0) {
			if (argv[i]) {
				segmentId = std::stoi(argv[i]);
				segments->push_back(segmentId);
			}
			else
			{
				std::cerr << "Segment id is NULL" << std::endl;
			}
		}
	}
	return 0;
}

static int dataQueryCallback(void *data, int argc, char **argv, char **azColName)
{
	std::vector<Common::TMeasuredValue*> *values = (std::vector<Common::TMeasuredValue*>*)data;

	int segmentId = NULL;
	double ist = NULL;
	double measureDate = NULL;


	int i;
	// v cyklu prochazime vsechny sloupecky, a hodnoty ukladame do Common::TMeasuredValue promenne
	// pokud je hodnota z databaze null, ulozi se do Common::TMeasuredValue take null
	for (i = 0; i < argc; i++) {
		//segment id
		if (strcmp(azColName[i], "segmentid") == 0) {
			if (argv[i]) {
				segmentId = std::stoi(argv[i]);
			}
			else {
				//std::cout << "Segment Id is NULL" << std::endl;
				segmentId = 0;
			}
		}
		//inzulin v krvi
		else if (strcmp(azColName[i], "ist") == 0) {
			if (argv[i]) {
				ist = std::stod(argv[i]);
			}
			else {
				//std::cout << "IST is NULL" << std::endl;
				ist = NULL;
			}
		}
		//datum mereni
		else if (strcmp(azColName[i], "measuredat") == 0) {
			if (argv[i]) {
				measureDate = std::stod(argv[i]);
			}
			else {
				std::cerr << "measuredate is NULL" << std::endl;
			}
		}
	}

	values->push_back(new Common::TMeasuredValue(&segmentId,&measureDate,&ist));

	return 0;
}

