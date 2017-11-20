#pragma once
#include "SegmentDay.h"
#include <memory>



namespace Common
{
	class SegmentDays
	{
	public:
		SegmentDays(std::shared_ptr<std::vector<std::shared_ptr<SegmentDay>>>& days);
		std::shared_ptr<std::vector<std::shared_ptr<SegmentDay>>> getDays();
	private:
		std::shared_ptr<std::vector<std::shared_ptr<SegmentDay>>> segmentDays;
	};

}

