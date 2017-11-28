#include "TMeasuredValue.h"

namespace Common
{
	TMeasuredValue::TMeasuredValue(int *segmentId, double *measureDate, double *ist)
	{
		this->segmentId = (*segmentId);
		this->measureDate = (*measureDate);
		this->ist = (*ist);
		this->setTimeFromMeasureDate(measureDate);
	}

	TMeasuredValue::~TMeasuredValue() 
	{
	}

	void TMeasuredValue::setTimeFromMeasureDate(double *measureDateToCalc)
	{
		double desetinne = (*measureDateToCalc) - (int)(*measureDateToCalc);
		int casVMinutach = (int)(desetinne*1440);
		this->hour = (int)(casVMinutach / 60);
		this->minutes = (int)(casVMinutach % 60);
	}

}