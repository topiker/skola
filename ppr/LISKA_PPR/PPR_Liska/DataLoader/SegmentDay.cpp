#include "SegmentDay.h"

namespace Common
{

	SegmentDay::SegmentDay(std::vector<TMeasuredValue *> **data, int *indexDay)
	{
		this->data = *data;
		this->dayIndex = (*indexDay);
	}
}
