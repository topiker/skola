#pragma once



namespace PeakPeakDetector 
{
	/// <summary>
	/// Trida slouzi jako prepravka dat o peaku
	/// </summary>
	class Peak
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="startIndex">Start index</param>
		/// <param name="endIndex">Koncovy index</param>
		/// <param name="fitness">Fitness hodnota okenka</param>
		Peak(unsigned int startIndex, unsigned int endIndex,double fitness);
		/// <summary>
		/// Start index
		/// </summary>
		size_t startIndex;
		/// <summary>
		/// Koncovy index
		/// </summary>
		size_t endIndex;
		/// <summary>
		/// Fitness hodnota okenka
		/// </summary>
		double fitness;
	private:
		
	};
}
