#include "DataLoader.h"
#include "sqlite3.h"
#include <iostream>
#include "SqlQueries.h"
#include "DatabaseConnector.h"
#include "FromTo.h"
#include <tbb\tbb.h>
#include "TMeasuredValue.h"

namespace DataLoader {

	DataLoader::DataLoader(char * dbPath)
	{
		this->dbPath = dbPath;
	}

	size_t DataLoader::getSegmentIds(std::vector<int> *segmentIds)
	{
		DatabaseConnector connector = DatabaseConnector::DatabaseConnector(&(this->dbPath));
		connector.executeCommand(segmentsQuery, segmentsQueryCallback, (void*)segmentIds);
		
		return (*segmentIds).size();
	}

	void DataLoader::loadDataBySegment(Common::Segment **result, int * segmentId)
	{
		
		std::string sqlStr = "SELECT (julianday(md.measuredat) - 2415019.5) AS 'measuredat', md.segmentid, md.ist FROM measuredvalue md inner join timesegment ts on md.segmentid = ts.id WHERE md.segmentId=" + std::to_string(*segmentId) + " ORDER BY md.measuredat";
		const char * sql = sqlStr.c_str();
		DatabaseConnector connector = DatabaseConnector::DatabaseConnector(&(this->dbPath));
		std::vector<Common::TMeasuredValue*> data = std::vector<Common::TMeasuredValue*>();
		connector.executeCommand(sql, dataQueryCallback, (void*)(&data));
		Common::SegmentDays *days = NULL;
		if (data.size() > 0) 
		{
			this->smoothNullValues(&data);
			this->splitIntoDays(&days,&data);
		}
		(*result) = new Common::Segment(segmentId, days);
	}

	void DataLoader::splitIntoDays(Common::SegmentDays **segmentDays, std::vector<Common::TMeasuredValue*> *data)
	{
		std::vector<FromTo> daysIndexes = this->getDaysIndexes(data);
		std::vector<Common::SegmentDay> *days = new std::vector<Common::SegmentDay>();
		int index = 0;
		for (size_t i = 0; i < daysIndexes.size(); i++)
		{
			FromTo current = daysIndexes.at(i);
			std::vector<Common::TMeasuredValue * > subdata = std::vector<Common::TMeasuredValue * >();
			for (size_t j = current.from; j < current.to; j++)
			{
				subdata.push_back((*data).at(j));
			}
			Common::SegmentDay *day = new Common::SegmentDay(&subdata, &(index));
			(*days).push_back(*day);
			index++;
		}
		(*segmentDays) = new Common::SegmentDays(days);
	}

	std::vector<FromTo> DataLoader::getDaysIndexes(std::vector<Common::TMeasuredValue*> *data)
	{
		std::vector<FromTo> fromTo = std::vector<FromTo>();
		size_t startIndex = 0;
		size_t dataSize = (*data).size();
		if (dataSize > 0)
		{
			for (size_t i = 0; i < dataSize; i++)
			{
				Common::TMeasuredValue *current = (*data).at(i);
				if ((*current).hour == 23)
				{
					for (size_t j = (i+1); j < dataSize; j++)
					{
						if ((*data).at(j)->hour == 0)
						{
							fromTo.push_back(FromTo(startIndex, j - 1));
							startIndex = j;
							i = j;
							break;
						}
					}
				}
			}

			if (!(startIndex >= dataSize))
			{
				fromTo.push_back(FromTo(startIndex, dataSize - 1));
			}

		}
		return fromTo;
	}

	void DataLoader::freeData(std::vector<Common::TMeasuredValue *> *data)
	{
		//for (size_t i = 0; i < (*data).size();i++)
		//{
		//	delete((*data).at(i));
		//}
	}

	size_t DataLoader::loadData(std::vector<Common::Segment> *data)
	{
		std::vector<int> segmentIds;
		this->getSegmentIds(&segmentIds);
		(*data) = std::vector<Common::Segment>();
		//for (size_t i = 0; i < segmentIds.size(); i++)
		//{
		//	Common::Segment *current;
		//	loadDataBySegment(&current, &((segmentIds).at(i)));
		//	(*data).push_back((*current));
		//}

		tbb::parallel_for(size_t(0), (segmentIds).size(), [&](size_t i) {
			Common::Segment *current;
			loadDataBySegment(&current, &((segmentIds).at(i)));
			(*data).push_back((*current));
		});
		
		return 0;
	}

	void DataLoader::smoothNullValues(std::vector<Common::TMeasuredValue *> *data)
	{
		double lastNonNullValue = 0;
		for (unsigned int i = 0; i < (*data).size(); i++) {
			if ((*data).at(i)->ist == NULL) {
				if (lastNonNullValue != 0) {
					(*data).at(i)->ist = lastNonNullValue;
				}
			}
			else {
				if (lastNonNullValue == 0 && i != 0) {
					for (int j = i; j >= 0; j--) {
						double ist = (*data).at(i)->ist;
						(*data).at(j)->ist = ist;
					}
				}
				lastNonNullValue = (*data).at(i)->ist;
			}
		}
	}


	DataLoader::~DataLoader()
	{
	}

}