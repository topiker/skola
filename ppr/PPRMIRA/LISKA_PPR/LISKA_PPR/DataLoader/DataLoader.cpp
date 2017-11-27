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

	void DataLoader::loadDataBySegment(std::unique_ptr<Common::Segment>& result, int * segmentId)
	{

		std::string sqlStr = "SELECT (julianday(md.measuredat) - 2415019.5) AS 'measuredat', md.segmentid, md.ist FROM measuredvalue md inner join timesegment ts on md.segmentid = ts.id WHERE md.segmentId=" + std::to_string(*segmentId) + " ORDER BY md.measuredat";
		const char * sql = sqlStr.c_str();
		DatabaseConnector connector = DatabaseConnector::DatabaseConnector(&(this->dbPath));
		std::vector<std::unique_ptr<Common::TMeasuredValue>> data = std::vector<std::unique_ptr<Common::TMeasuredValue>>();
		connector.executeCommand(sql, dataQueryCallback, (void*)(&data));
		std::unique_ptr<Common::SegmentDays> days = std::unique_ptr<Common::SegmentDays>();
		std::unique_ptr<std::vector<std::unique_ptr<Common::TMeasuredValue>>> dataLoaded = std::make_unique<std::vector<std::unique_ptr<Common::TMeasuredValue>>>(std::move(data));
		if (dataLoaded.get()->size() > 0)
		{
			this->smoothNullValues(dataLoaded);
			this->splitIntoDays(days, dataLoaded);
		}
		(result) = std::unique_ptr<Common::Segment>(new Common::Segment(segmentId, days));
	}

	void DataLoader::splitIntoDays(std::unique_ptr<Common::SegmentDays>& segmentDays, const std::unique_ptr<std::vector<std::unique_ptr<Common::TMeasuredValue>>>& data)
	{
		std::vector<FromTo> daysIndexes = this->getDaysIndexes(data);
		std::unique_ptr<std::vector<std::unique_ptr<Common::SegmentDay>>> days = std::make_unique<std::vector<std::unique_ptr<Common::SegmentDay>>>();
		int index = 0;
		for (size_t i = 0; i < daysIndexes.size(); i++)
		{
			FromTo current = daysIndexes.at(i);
			auto subdata = std::make_unique<std::vector<std::unique_ptr<Common::TMeasuredValue>>>();
			for (size_t j = current.from; j < current.to; j++)
			{
				(*subdata).push_back(std::move((*data).at(j)));
			}
			(*days).push_back(std::unique_ptr<Common::SegmentDay>(new Common::SegmentDay(subdata, &(index))));
			//double neco  =(*subdata).at(0).get()->measureDate;
			index++;
		}
		(segmentDays) = std::unique_ptr<Common::SegmentDays>(new Common::SegmentDays(days));
	}

	std::vector<FromTo> DataLoader::getDaysIndexes(const std::unique_ptr<std::vector<std::unique_ptr<Common::TMeasuredValue>>>& data)
	{
		std::vector<FromTo> fromTo = std::vector<FromTo>();
		size_t startIndex = 0;
		size_t dataSize = (*data).size();
		if (dataSize > 0)
		{
			for (size_t i = 0; i < dataSize; i++)
			{
				auto current = (data).get()->at(i).get();
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

	//void DataLoader::freeData(std::vector<Common::TMeasuredValue *> *data)
	//{
	//	//for (size_t i = 0; i < (*data).size();i++)
	//	//{
	//	//	delete((*data).at(i));
	//	//}
	//}

	size_t DataLoader::loadData(std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data)
	{
		std::vector<int> segmentIds;
		this->getSegmentIds(&segmentIds);
		(data) = std::make_unique<std::vector<std::unique_ptr<Common::Segment>>>();
		for (size_t i = 0; i < segmentIds.size(); i++)
		{
			std::unique_ptr<Common::Segment> current = std::unique_ptr<Common::Segment>();
			loadDataBySegment(current, &((segmentIds).at(i)));
			(data.get()->push_back(std::move(current)));
		}

		//(data) = std::make_unique<std::vector<std::unique_ptr<Common::Segment>>>();
		//tbb::parallel_for(size_t(0), (segmentIds).size(), [&](size_t i) {
		//	std::unique_ptr<Common::Segment> current = std::unique_ptr<Common::Segment>();
		//	loadDataBySegment(current, &((segmentIds).at(i)));
		//	(*data).push_back(current);
		//});
		
		return 0;
	}

	void DataLoader::smoothNullValues(std::unique_ptr<std::vector<std::unique_ptr<Common::TMeasuredValue>>>& data)
	{
		double lastNonNullValue = 0;
		for (unsigned int i = 0; i < (*data).size(); i++) {
			if ((*data).at(i).get()->ist == NULL) {
				if (lastNonNullValue != 0) {
					(*data).at(i).get()->ist = lastNonNullValue;
				}
			}
			else {
				if (lastNonNullValue == 0 && i != 0) {
					for (int j = i; j >= 0; j--) {
						double ist = (*data).at(i).get()->ist;
						(*data).at(j).get()->ist = ist;
					}
				}
				lastNonNullValue = (*data).at(i).get()->ist;
			}
		}
	}


	DataLoader::~DataLoader()
	{
	}

}