#pragma once
#include "SegmentDay.h"
#include <memory>



namespace Common
{
	/// <summary>
	/// Trida slouzi jako obalovaci trida pro uchovani dat jednotlivych dnu
	/// </summary>
	class SegmentDays
	{
	public:
		/// <summary>
		/// Vstupem jsou data rozdelena po dnech
		/// </summary>
		/// <param name="days"></param>
		SegmentDays(std::unique_ptr<std::vector<std::unique_ptr<SegmentDay>>>& days);
		/// <summary>
		/// Vraci data rozdelena po dnech
		/// </summary>
		/// <returns></returns>
		std::vector<std::unique_ptr<SegmentDay>>* getDays();
		/// <summary>
		/// Vraci pocet dni
		/// </summary>
		/// <returns></returns>
		size_t getDaysSize();
	private:
		/// <summary>
		/// Data rozdelena po dnech
		/// </summary>
		std::unique_ptr<std::vector<std::unique_ptr<SegmentDay>>> segmentDays;
	};

}

