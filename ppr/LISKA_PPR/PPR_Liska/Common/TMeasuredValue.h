#pragma once

typedef struct {
	int id;
	int segmentId;
	double measureDate; //v julian
	double ist; //namerene hodnoty
	bool isInWindow;
	double smoothedValue;

}TMeasuredValue;


