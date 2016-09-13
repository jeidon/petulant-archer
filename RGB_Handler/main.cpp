#include "bmp_handler.h"

std::string APPPATH;

int main(int argc, char *argv[])
{
	//Get this party started
	_bmpImage image;
	
	//Where is the app running from?
	APPPATH = argv[0];

	if (argc == 1)
	{
		printf("This utility requires a source file in order to perform a conversion\n");
		system("pause");
	}
	else
	{

		//Where is the source file?
		image.setSourceFile(argv[1]);

		//Open and fill the buffer with the image
		image.openSourceFile();

		//Create header files in each of the possible formats
		image.createHeaderFile(bit8, 1);
		image.createHeaderFile(bit8, 2);
		image.createHeaderFile(bit16, 1);
		image.createHeaderFile(bit16, 2);
		image.createHeaderFile(bit18, 1);
		image.createHeaderFile(bit24, 2);
		image.createHeaderFile(bit32, 1);
		image.createHeaderFile(bit18, 2);
		image.createHeaderFile(bit24, 1);
		image.createHeaderFile(bit32, 2);
	}
}