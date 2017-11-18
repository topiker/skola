#pragma once
#include "TMeasuredValue.h"
#include <vector>

namespace Common
{
	/// <summary>
	/// Reprezentuje jeden den mereni v segmentu
	/// </summary>
	class SegmentDay
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="data">Data pro dany den</param>
		/// <param name="indexDay">Index dne</param>
		SegmentDay(std::vector<TMeasuredValue *> *data, int *indexDay);
		/// <summary>
		/// Index dne
		/// </summary>
		int dayIndex;
		/// <summary>
		/// Data pro dany den
		/// </summary>
		std::vector<TMeasuredValue *> data;
	};
}

