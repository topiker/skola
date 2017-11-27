#include "DataWrapper.h"

namespace Common
{


	DataWrapper::DataWrapper(std::vector<std::vector<TMeasuredValue *>> *data)
	{
		this->data = *data;
	}


	DataWrapper::~DataWrapper()
	{
		if (!(this->data.empty()))
		{
			for (size_t i = 0; i < (data).size(); i++)
			{
				delete(&((data).at(i)));
				(data).at(i).clear();
			}
			(data).clear();
		}
		
	}

	std::vector<std::vector<TMeasuredValue *>>* DataWrapper::getData()
	{
		return &(this->data);
	}
}
