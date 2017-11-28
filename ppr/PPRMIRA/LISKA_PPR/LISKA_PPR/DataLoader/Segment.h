#pragma once
#include "SegmentDays.h"
#include <memory>


namespace Common {

	/// <summary>
	/// Obalovaci trida pro segment v datech. Segment je rozdelen na dny
	/// </summary>
	class Segment
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="segmentId">Id segmentu</param>
		/// <param name="days">data segmentu rozdelena po dnech</param>
		Segment(int *segmentId, std::unique_ptr<SegmentDays>& days);
		/// <summary>
		/// Vraci ukazatel na data rozdelena po dnech
		/// </summary>
		/// <returns></returns>
		SegmentDays* getSegmentDays();
		/// <summary>
		/// Vraci segment Id
		/// </summary>
		/// <returns></returns>
		int getSegmentId();
	private:
		/// <summary>
		/// Id segmentu
		/// </summary>
		int segmentId;
		/// <summary>
		/// Dny segmentu
		/// </summary>
		std::unique_ptr<SegmentDays> segmentDays;
	};

}

