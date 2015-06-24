#include <iostream>

#define DEBUG 1

float linearConvert(const unsigned int x, float A, float B, float C, float D);
float downconvert(const unsigned int start);
float upconvert(const unsigned int start);

int main(void)
{
	unsigned int start = 0x85;
	unsigned int end;

	end = upconvert(start);

	start = downconvert(end);

	system("pause");
}

float downconvert(const unsigned int start)
{
	// Need to split the three colors and upconvert them individually
	// since the three colors each have distinct ranges
	float R, G, B;
	unsigned int newValue;

	// Assuming the source image's color config is RRRGGGBB
	// Assuming the desired color config is RRRRRGGGGGGBBBBB

#if(DEBUG == 1)
	printf("Starting Value: %i\n\n", start);
#endif
	//Shift apart the individual colors
	R = (start & 0xF800)>>11;
	G = (start & 0x07E0)>>5;
	B = (start & 0x001F);
	
#if DEBUG == 1
	printf("Initial RGB Values:\n\tR = %f\n\tG = %f\n\tB = %f\n", R, G, B);
#endif
	//The ranges for R are [0-31] and [0-7]
	R = linearConvert(R, 0, 31, 0, 7);
	//The ranges for G are [0-63] and [0-7]
	G = linearConvert(G, 0, 63, 0, 7);
	//The ranges for B are [0-31] and [0-3]
	B = linearConvert(B, 0, 31, 0, 3);

#if DEBUG == 1
	printf("After Linear Conversion:\n\tR = %f\n\tG = %f\n\tB = %f\n", R, G, B);
#endif
	//Shift them back into place
	R = (int)R<<5;
	G = (int)G<<2;
	B = (int)B;

#if DEBUG == 1
	printf("Shifted back up:\n\tR = %f\n\tG = %f\n\tB = %f\n", R, G, B);
#endif
	newValue = R + G + B;

//#if DEBUG == 1
	printf("\nDownscaled: %i\n", newValue);
//#endif
 	return newValue;
}

float upconvert(const unsigned int start)
{
	// Need to split the three colors and upconvert them individually
	// since the three colors each have distinct ranges
	float R, G, B;
	unsigned int newValue;

	// Assuming the source image's color config is RRRGGGBB
	// Assuming the desired color config is RRRRRGGGGGGBBBBB

#if(DEBUG == 1)
	printf("Starting Value: %i\n\n", start);
#endif
	//Shift apart the individual colors
	R = (start & 0xE0)>>5;
	G = (start & 0x1C)>>2;
	B = (start & 0x03);
	
#if DEBUG == 1
	printf("Initial RGB Values:\n\tR = %f\n\tG = %f\n\tB = %f\n", R, G, B);
#endif
	//The ranges for R are [0-7] and [0-31]
	R = linearConvert(R, 0, 7, 0, 31);
	//The ranges for G are [0-7] and [0-63]
	G = linearConvert(G, 0, 7, 0, 63);
	//The ranges for B are [0-3] and [0-31]
	B = linearConvert(B, 0, 3, 0, 31);

#if DEBUG == 1
	printf("After Linear Conversion:\n\tR = %f\n\tG = %f\n\tB = %f\n", R, G, B);
#endif
	//Shift them back into place
	R = (int)R<<11;
	G = (int)G<<5;
	B = (int)B;

#if DEBUG == 1
	printf("Shifted back up:\n\tR = %f\n\tG = %f\n\tB = %f\n", R, G, B);
#endif
	newValue = R + G + B;

//#if DEBUG == 1
	printf("\nUpscaled: %i\n", newValue);
//#endif
 	return newValue;
}

float linearConvert(const unsigned int x, float A, float B, float C, float D)
{
	float returnValue;
	
	// This equation came from a stack exchange post
	//http://math.stackexchange.com/questions/43698/range-scaling-problem
	returnValue = C + (D-C)*((x-A)/(B-A));
	return returnValue;
}