#pragma once
#pragma once

#include <iostream>
#include <vector>
#include <string>


const char *segmentsQuery = "SELECT id FROM timesegment ORDER BY id ASC;";
const char *measuredvalueQuery = "SELECT (julianday(measuredat) - 2415019.5) AS 'measuredat',id, segmentid, ist FROM measuredvalue WHERE segmentId=%i ORDER BY measuredat;";

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