#include "SegmentDay.h"

namespace Common
{

	SegmentDay::SegmentDay(std::vector<TMeasuredValue *> *data, int *indexDay)
	{
		this->data = *data;
		this->dayIndex = (*indexDay);
	}

	std::vector<TMeasuredValue *>* SegmentDay::getData()
	{
		return &(this->data);
	}

}
