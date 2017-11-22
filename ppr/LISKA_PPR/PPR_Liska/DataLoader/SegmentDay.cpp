#include "SegmentDay.h"

namespace Common
{

	SegmentDay::SegmentDay(std::unique_ptr<std::vector<std::unique_ptr<TMeasuredValue>>>& data, int *indexDay):data(std::move(data))
	{
		this->dayIndex = (*indexDay);
	}

	std::vector<std::unique_ptr<TMeasuredValue>>* SegmentDay::getData()
	{
		return this->data.get();
	}

}
