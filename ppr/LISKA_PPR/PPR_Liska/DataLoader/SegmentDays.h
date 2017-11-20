#pragma once
#include "SegmentDay.h"



namespace Common
{
	class SegmentDays
	{
	public:
		SegmentDays(std::vector<SegmentDay> *days);
		std::vector<SegmentDay>* getDays();
	private:
		std::vector<SegmentDay> *segmentDays;
	};

}

