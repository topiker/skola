#pragma once
#include "SegmentDays.h"


namespace Common {

	class Segment
	{
	public:
		Segment();
		Segment(int *segmentId, SegmentDays *days);
		SegmentDays* getSegmentDays();
		//void clearAfterMe();
		int* getSegmentId();
	private:
		int segmentId;
		SegmentDays *segmentDays;
	};

}

