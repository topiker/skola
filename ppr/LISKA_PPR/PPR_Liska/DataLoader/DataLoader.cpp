#include "DataLoader.h"
#include "sqlite3.h"
#include <iostream>
#include "SqlQueries.h"
#include "DatabaseConnector.h"
#include "FromTo.h"
#include "SegmentDay.h"

namespace DataLoader {

	DataLoader::DataLoader(char * dbPath)
	{
		this->dbPath = dbPath;
	}

	size_t DataLoader::getSegmentIds(std::vector<int> *segmentIds)
	{
		std::cout << "Ziskavam id segmentu" << std::endl;
		DatabaseConnector connector = DatabaseConnector::DatabaseConnector(&(this->dbPath));
		connector.executeCommand(segmentsQuery, segmentsQueryCallback, (void*)segmentIds);
		
		return (*segmentIds).size();
	}

	size_t DataLoader::loadData(std::vector<Common::TMeasuredValue*> *data, int * segmentId)
	{
		std::string sqlStr = "SELECT (julianday(measuredat) - 2415019.5) AS 'measuredat',id, segmentid, ist FROM measuredvalue WHERE segmentId=" + std::to_string(*segmentId) + " ORDER BY measuredat";
		const char * sql = sqlStr.c_str();
		DatabaseConnector connector = DatabaseConnector::DatabaseConnector(&(this->dbPath));
		connector.executeCommand(sql, dataQueryCallback, (void*)data);

		//TODO: Rozdelit po dnech.
		//Podle prvniho datumu

		//std::vector<FromTo> daysIndexes = this->getDaysIndexes(&data);
		//(*days) = std::vector<Common::SegmentDay>();
		//int index = 0;
		//for (unsigned int i = 0; i < daysIndexes.size(); i++)
		//{
		//	FromTo current = daysIndexes.at(i);
		//	std::vector<Common::TMeasuredValue * > subdata = std::vector<Common::TMeasuredValue * >();
		//	for (size_t j = current.from; j < current.to; j++)
		//	{
		//		subdata.push_back((data).at(j));
		//	}
		//	Common::SegmentDay day = Common::SegmentDay(&subdata, &(index));
		//	(*days).push_back(day);
		//	index++;
		//}

		return (*data).size();
	}

	void DataLoader::splitIntoDays(std::vector<Common::SegmentDay> *days, std::vector<Common::TMeasuredValue*> *data)
	{
		std::vector<FromTo> daysIndexes = this->getDaysIndexes(data);
		(*days) = std::vector<Common::SegmentDay>();
		int index = 0;
		for (unsigned int i = 0; i < daysIndexes.size(); i++)
		{
			FromTo current = daysIndexes.at(i);
			std::vector<Common::TMeasuredValue * > subdata = std::vector<Common::TMeasuredValue * >();
			for (size_t j = current.from; j < current.to; j++)
			{
				subdata.push_back((*data).at(j));
			}
			Common::SegmentDay day = Common::SegmentDay(&subdata, &(index));
			(*days).push_back(day);
			index++;
		}
	}

	std::vector<FromTo> DataLoader::getDaysIndexes(std::vector<Common::TMeasuredValue*> *data)
	{
		std::vector<FromTo> fromTo = std::vector<FromTo>();
		size_t startIndex = 0;
		size_t dataSize = (*data).size();
		if (dataSize > 0)
		{
			unsigned int i = 0;
			for (i; i < dataSize; i++)
			{
				Common::TMeasuredValue *value = (*data).at(i);
				if (i != 0 && ((*value).hour == 23))
				{
					for (unsigned int j = i; j < dataSize - 1; j++)
					{
						if ((*data).at(j + 1)->hour == 0)
						{
							fromTo.push_back(FromTo(startIndex, j));
							i = j + 1;
							startIndex = j + 1;
							break;
						}
					}
				}
			}
			if (startIndex != (dataSize - 1))
			{
				fromTo.push_back(FromTo(startIndex, dataSize - 1));
			}
		}
		return fromTo;
	}


	DataLoader::~DataLoader()
	{
	}

}