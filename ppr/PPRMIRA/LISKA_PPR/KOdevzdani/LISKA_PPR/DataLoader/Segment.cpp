#include "Segment.h"


namespace Common
{

	Segment::Segment(int *segmentId, std::unique_ptr<SegmentDays>& days):segmentDays(std::move(days))
	{
		this->segmentId = (*segmentId);
	}

	SegmentDays* Segment::getSegmentDays()
	{
		return (this->segmentDays).get();
	}

	int Segment::getSegmentId()
	{
		return (this->segmentId);
	}

}