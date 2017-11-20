#include "SegmentDay.h"

namespace Common
{

	SegmentDay::SegmentDay(std::shared_ptr<std::vector<std::shared_ptr<TMeasuredValue>>>& data, int *indexDay)
	{
		this->data = data;
		this->dayIndex = (*indexDay);
	}

	std::shared_ptr<std::vector<std::shared_ptr<TMeasuredValue>>> SegmentDay::getData()
	{
		return (this->data);
	}

}
