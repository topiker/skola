#pragma once
#include "TMeasuredValue.h"
#include <vector>
class DataHelper
{
public:
	static double getMaxIstValue(std::vector<Common::TMeasuredValue*>  *values);
};

