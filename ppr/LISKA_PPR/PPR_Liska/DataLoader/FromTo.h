#pragma once
namespace DataLoader {
	/// <summary>
	/// Trida slouzi jako prepravka pro uchovani rozsahu od do
	/// </summary>
	class FromTo
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="from">Od</param>
		/// <param name="to">Do</param>
		FromTo(size_t from, size_t to);
		/// <summary>
		/// Od
		/// </summary>
		size_t from;
		/// <summary>
		/// Do
		/// </summary>
		size_t to;
	};
}

