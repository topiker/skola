#pragma once
#include "TMeasuredValue.h"
#include <vector>
namespace Common
{
	class SegmentDay
	{
	public:
		SegmentDay(int *data, int *indexDay);
		//SegmentDay(std::vector<TMeasuredValue *> *data, int *indexDay);
	private:
		int dayIndex;
		std::vector<TMeasuredValue *> *data;
	};
}

