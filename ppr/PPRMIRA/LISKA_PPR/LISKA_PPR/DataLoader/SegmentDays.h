#pragma once
#include "SegmentDay.h"
#include <memory>



namespace Common
{
	class SegmentDays
	{
	public:
		SegmentDays(std::unique_ptr<std::vector<std::unique_ptr<SegmentDay>>>& days);
		std::vector<std::unique_ptr<SegmentDay>>* getDays();
		size_t getDaysSize();
		int segmentId;
	private:
		std::unique_ptr<std::vector<std::unique_ptr<SegmentDay>>> segmentDays;
	};

}

