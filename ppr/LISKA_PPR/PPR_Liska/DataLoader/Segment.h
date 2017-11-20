#pragma once
#include "SegmentDays.h"
#include <memory>


namespace Common {

	class Segment
	{
	public:
		Segment(int *segmentId, std::shared_ptr<SegmentDays>& days);
		std::shared_ptr<SegmentDays> getSegmentDays();
		int getSegmentId();
	private:
		int segmentId;
		std::shared_ptr<SegmentDays> segmentDays;
	};

}

