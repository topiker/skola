#pragma once



namespace PeakPeakDetector 
{
	class Peak
	{
	public:
		Peak(unsigned int startIndex, unsigned int endIndex,double fitness);
		size_t startIndex;
		size_t endIndex;
		double fitness;
	private:
		
	};
}
