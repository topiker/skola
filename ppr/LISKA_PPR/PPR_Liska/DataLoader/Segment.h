#pragma once
#include "SegmentDays.h"
#include <memory>


namespace Common {

	class Segment
	{
	public:
		Segment(int *segmentId, std::unique_ptr<SegmentDays>& days);
		SegmentDays* getSegmentDays();
		int getSegmentId();
	private:
		int segmentId;
		std::unique_ptr<SegmentDays> segmentDays;
	};

}

