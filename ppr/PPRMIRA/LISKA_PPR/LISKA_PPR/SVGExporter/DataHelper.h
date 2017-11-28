#pragma once
#include "TMeasuredValue.h"
#include <vector>
class DataHelper
{
public:
	/// <summary>
	/// Ze vstupnich dat ziska maximalni IST hodnotu
	/// </summary>
	/// <param name="values"></param>
	/// <returns></returns>
	static double getMaxIstValue(std::vector<Common::TMeasuredValue*>  *values);
};

