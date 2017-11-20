#include "Segment.h"


namespace Common
{

	Segment::Segment(int *segmentId, std::shared_ptr<SegmentDays>& days)
	{
		this->segmentDays = days;
		this->segmentId = (*segmentId);
	}

	std::shared_ptr<SegmentDays> Segment::getSegmentDays()
	{
		return (this->segmentDays);
	}

	int Segment::getSegmentId()
	{
		return (this->segmentId);
	}

}