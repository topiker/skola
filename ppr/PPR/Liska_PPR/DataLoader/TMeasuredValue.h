#pragma once




namespace Common
{
	class TMeasuredValue {
	public:
		TMeasuredValue(int *segmentId, double *measureDate, double *ist);
		~TMeasuredValue();
		double ist;
		double measureDate; //v julian
		int hour;
		int minutes;
		int segmentId;
	private:
		void setTimeFromMeasureDate(double *measureDate);
	};
}


