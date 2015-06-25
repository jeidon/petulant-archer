#include "shared.h"

BYTE linearConvert(const BYTE x, float A, float B, float C, float D)
{
	BYTE returnValue;
	
	// This equation came from a stack exchange post
	//http://math.stackexchange.com/questions/43698/range-scaling-problem
	returnValue = (BYTE)(C + (D-C)*((x-A)/(B-A)));
	return returnValue;
}