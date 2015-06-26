#include "bmp_handler.h"

#define DEBUG 1

std::string APPPATH;

int main(int argc, char *argv[])
{
	//This is a hack to simulate with mock arguments
	argc = 3;
	argv[1] = "D:\\Projects\\petulant-archer\\test_files\\compass_color.bmp";
	argv[2] = "2";
	//End of HACK

	//Get this party started
	_bmpImage image;
	
	//Where is the app running from?
	APPPATH = argv[0];

	//Where is the source file?
	image.setSourceFile(argv[1]);

	//How should the resultant files store the values?
	image.dimensions = *argv[2] - '0';

	//Open and fill the buffer with the image
	image.openSourceFile();

	//Create header files in each of the possible sizes
	image.createHeaderFile(bit8);
	image.createHeaderFile(bit16);
	image.createHeaderFile(bit18);
	image.createHeaderFile(bit24);
	image.createHeaderFile(bit32);

	system("pause");
}