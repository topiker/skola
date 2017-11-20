#include "SegmentDays.h"

namespace Common
{


	SegmentDays::SegmentDays(std::shared_ptr<std::vector<std::shared_ptr<SegmentDay>>> &days)
	{
		this->segmentDays = days;
	}


	std::shared_ptr<std::vector<std::shared_ptr<SegmentDay>>> SegmentDays::getDays()
	{
		return this->segmentDays;
	}
}

