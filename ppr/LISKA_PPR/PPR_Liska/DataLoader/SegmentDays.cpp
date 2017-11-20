#include "SegmentDays.h"

namespace Common
{


	SegmentDays::SegmentDays(std::vector<SegmentDay> *days)
	{
		this->segmentDays = days;
	}


	std::vector<SegmentDay>* SegmentDays::getDays()
	{
		return this->segmentDays;
	}
}

