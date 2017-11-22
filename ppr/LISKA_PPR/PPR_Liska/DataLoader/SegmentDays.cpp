#include "SegmentDays.h"

namespace Common
{


	SegmentDays::SegmentDays(std::unique_ptr<std::vector<std::unique_ptr<SegmentDay>>> &days):segmentDays(std::move(days))
	{

	}


	std::vector<std::unique_ptr<SegmentDay>>* SegmentDays::getDays()
	{
		return this->segmentDays.get();
	}

	size_t SegmentDays::getDaysSize() 
	{
		return this->segmentDays.get()->size();
	}
}

