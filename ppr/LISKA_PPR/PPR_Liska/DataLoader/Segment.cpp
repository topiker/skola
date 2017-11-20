#include "Segment.h"


namespace Common
{

	Segment::Segment(int *segmentId, SegmentDays *days) 
	{
		this->segmentDays = days;
		this->segmentId = (*segmentId);
	}

	Segment::Segment()
	{
		this->segmentId = NULL;
		this->segmentDays = NULL;
	}

	//Segment::~Segment() 
	//{
	//	if (this->segmentDays != NULL) 
	//	{
	//		for (size_t i = 0; i < this->getSegmentDays()->getDays()->size(); i++)
	//		{
	//			for (size_t j = 0; j < this->getSegmentDays()->getDays()->at(i).getData()->size(); j++)
	//			{
	//				free(this->getSegmentDays()->getDays()->at(i).getData()->at(j));

	//			}
	//			this->getSegmentDays()->getDays()->at(i).getData()->clear();
	//		}
	//	}
	//}

	SegmentDays* Segment::getSegmentDays()
	{
		return (this->segmentDays);
	}

	int* Segment::getSegmentId()
	{
		return &(this->segmentId);
	}

}