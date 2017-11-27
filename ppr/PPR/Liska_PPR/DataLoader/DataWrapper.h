#pragma once
#include "TMeasuredValue.h"
#include <vector>

namespace Common {

	class DataWrapper
	{
	public:
		DataWrapper(std::vector<std::vector<TMeasuredValue *>> *data);
		~DataWrapper();
	private:
		std::vector<std::vector<TMeasuredValue *>>* getData();
		std::vector<std::vector<TMeasuredValue *>> data;

	};
}


