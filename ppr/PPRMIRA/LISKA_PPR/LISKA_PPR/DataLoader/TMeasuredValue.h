#pragma once




namespace Common
{
	/// <summary>
	/// Trida slouzi jako prepraka namerenych dat z database
	/// </summary>
	class TMeasuredValue {
	public:
		/// <summary>
		/// Zaroven spocita z measure data hodinu a minutu mereni
		/// </summary>
		/// <param name="segmentId">Id segmentu</param>
		/// <param name="measureDate">Datum mereni</param>
		/// <param name="ist">namerena hodnota</param>
		TMeasuredValue(int *segmentId, double *measureDate, double *ist);
		~TMeasuredValue();
		/// <summary>
		/// Namerena hodnota
		/// </summary>
		double ist;
		/// <summary>
		/// Doba mereni v julian
		/// </summary>
		double measureDate; //v julian
		/// <summary>
		/// Hodina mereni
		/// </summary>
		int hour;
		/// <summary>
		/// Minuta mereni
		/// </summary>
		int minutes;
		/// <summary>
		/// Id segmentu
		/// </summary>
		int segmentId;
	private:
		/// <summary>
		/// Prevadit measure date v julian na hodiny a minuty. Nastavi je atributum tridy.
		/// </summary>
		/// <param name="measureDate"></param>
		void setTimeFromMeasureDate(double *measureDate);
	};
}


