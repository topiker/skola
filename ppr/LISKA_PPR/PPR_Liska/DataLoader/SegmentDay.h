#pragma once
#include "TMeasuredValue.h"
#include <vector>

namespace Common
{
	class SegmentDay
	{
	public:
		SegmentDay(std::vector<TMeasuredValue *> *data, int *indexDay);
		int dayIndex;
		std::vector<TMeasuredValue *> data;
	};
}

