#pragma once
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
		if (argv[i]) {
			segmentId = std::stoi(argv[i]);
			segments->push_back(segmentId);
		}
		else
		{
			std::cerr << "Segment id is NULL" << std::endl;
		}
	}
	return 0;
}

static int dataQueryCallback(void *data, int argc, char **argv, char **azColName)
{
	std::vector<TMeasuredValue*> *values = (std::vector<TMeasuredValue*>*)data;

	TMeasuredValue *value = (TMeasuredValue*)malloc(sizeof(TMeasuredValue));
	if (value != nullptr) {
		int i;
		// v cyklu prochazime vsechny sloupecky, a hodnoty ukladame do TmeasuredValue promenne
		// pokud je hodnota z databaze null, ulozi se do TMeasuredValue take null
		for (i = 0; i < argc; i++) {
			//id
			if (strcmp(azColName[i], "id") == 0) {
				if (argv[i]) {
					value->id = std::stoi(argv[i]);
				}
				else {
					std::cerr << "Id is NULL" << std::endl;
					value->id = 0;
				}
			}
			//segment id
			else if (strcmp(azColName[i], "segmentid") == 0) {
				if (argv[i]) {
					value->segmentId = std::stoi(argv[i]);
				}
				else {
					//std::cout << "Segment Id is NULL" << std::endl;
					value->segmentId = 0;
				}
			}
			//inzulin v krvi
			else if (strcmp(azColName[i], "ist") == 0) {
				if (argv[i]) {
					value->ist = std::stod(argv[i]);
				}
				else {
					//std::cout << "IST is NULL" << std::endl;
					value->ist = NULL;
				}
			}
			//datum mereni
			else if (strcmp(azColName[i], "measuredat") == 0) {
				if (argv[i]) {
					double measureDate = std::stod(argv[i]);
					//Zajima mi pouze cas, nepotreba tedy dale upravovat.
					value->measureDate = measureDate;
				}
				else {
					std::cerr << "measuredate is NULL" << std::endl;
				}
			}
		}
		values->push_back(value);
	}
	else
	{
		std::cerr << "Failed to alloc memory" << std::endl;
	}
	return 0;
}

