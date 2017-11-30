#include "DataHelper.h"



double DataHelper::getMaxIstValue(std::vector<Common::TMeasuredValue*>  *values)
{

	/*Zjisteni nejvetsi a nejmensi ist hodnoty v datech*/
	double maxMmolValue = -DBL_MAX;
	for each (Common::TMeasuredValue *item in *values)
	{
		if ((item)->ist > maxMmolValue)
		{
			maxMmolValue = item->ist;
		}
	}

	return maxMmolValue;
}
