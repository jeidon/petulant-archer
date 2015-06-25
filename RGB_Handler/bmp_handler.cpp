#include "bmp_handler.h"

int _bmpImage::createHeaderFiles()
{
	FILE *fDest32, *fDest24, *fDest18, *fDest16, *fDest8;
	std::string destfile, tempFilename;
	
	destfile = imageDir;
	destfile += imageString;

	tempFilename = destfile;
	tempFilename += "_32bit.h";
	if((fDest32 = fopen(tempFilename.c_str(), "wb")) == NULL)
	{
 		printf("The file %s cannot be created\n", tempFilename.c_str());
		exit(0);
	}

	tempFilename = destfile;
	tempFilename += "_24bit.h";
	if((fDest24 = fopen(tempFilename.c_str(), "wb")) == NULL)
	{
 		printf("The file %s cannot be created\n", tempFilename.c_str());
		exit(0);
	}

	tempFilename = destfile;
	tempFilename += "_18bit.h";
	if((fDest18 = fopen(tempFilename.c_str(), "wb")) == NULL)
	{
 		printf("The file %s cannot be created\n", tempFilename.c_str());
		exit(0);
	}

	tempFilename = destfile;
	tempFilename += "_16bit.h";
	if((fDest16 = fopen(tempFilename.c_str(), "wb")) == NULL)
	{
 		printf("The file %s cannot be created\n", tempFilename.c_str());
		exit(0);
	}

	tempFilename = destfile;
	tempFilename += "_8bit.h";
	if((fDest8 = fopen(tempFilename.c_str(), "wb")) == NULL)
	{
 		printf("The file %s cannot be created\n", tempFilename.c_str());
		exit(0);
	}

	//Print the opening that makes this a variable
	fprintf(fDest32, "PROGMEM const unsigned char %s_32bit", imageString.c_str());
	fprintf(fDest24, "PROGMEM const unsigned char %s_24bit", imageString.c_str());
	fprintf(fDest18, "PROGMEM const unsigned char %s_18bit", imageString.c_str());
	fprintf(fDest16, "PROGMEM const unsigned char %s_16bit", imageString.c_str());
	fprintf(fDest8, "PROGMEM const unsigned char %s_8bit", imageString.c_str());

	if(dimensions == 2)
	{
		fprintf(fDest32, "[%i][%i]", bih.biHeight, bih.biWidth*4);
		fprintf(fDest24, "[%i][%i]", bih.biHeight, bih.biWidth*3);
		fprintf(fDest18, "[%i][%i]", bih.biHeight, bih.biWidth*3);
		fprintf(fDest16, "[%i][%i]", bih.biHeight, bih.biWidth*2);
		fprintf(fDest8, "[%i][%i]", bih.biHeight, bih.biWidth);
	}
	fprintf(fDest32, " = {\n");
	fprintf(fDest24, " = {\n");
	fprintf(fDest18, " = {\n");
	fprintf(fDest16, " = {\n");
	fprintf(fDest8, " = {\n");

 	long Line = 0;
 	

	for(LONG y = 0; y < bih.biWidth; y++)
	{
		for(LONG x = 0; x < bih.biHeight; x++)
		{
			BYTE newValue = 0;
			BYTE msb, lsb;
			RGBTRIPLE pixel = getPixel(x, y);
			RGBTRIPLE newPixel;
			
			if( (Line==0 || Line%bih.biWidth == 0) && (dimensions == 2) )
			{
				fprintf(fDest32, "{");
				fprintf(fDest24, "{");
				fprintf(fDest18, "{");
				fprintf(fDest16, "{");
				fprintf(fDest8, "{");
			}

			//32Bit Easy... done
			fprintf(fDest32, "0x%02x, ", alpha);
			fprintf(fDest32, "0x%02x, ", pixel.rgbtRed);
			fprintf(fDest32, "0x%02x, ", pixel.rgbtGreen);
			fprintf(fDest32, "0x%02x, ", pixel.rgbtBlue);

			//24Bit
			fprintf(fDest24, "0x%02x, ", pixel.rgbtRed);
			fprintf(fDest24, "0x%02x, ", pixel.rgbtGreen);
			fprintf(fDest24, "0x%02x, ", pixel.rgbtBlue);

			//18Bit
			downConvert18(&pixel, &newPixel);
			fprintf(fDest18, "0x%02x, ", newPixel.rgbtRed);
			fprintf(fDest18, "0x%02x, ", newPixel.rgbtGreen);
			fprintf(fDest18, "0x%02x, ", newPixel.rgbtBlue);

			//16Bit
			downConvert16(&pixel, &msb, &lsb);
			fprintf(fDest16, "0x%02x, ", msb);
			fprintf(fDest16, "0x%02x, ", lsb);

			//8 bit
			downConvert8(&pixel, &newValue);
			fprintf(fDest8, "0x%02x, ", newValue);
 
			if(++Line==0 || Line%bih.biWidth == 0)
			{
				if(dimensions == 2)
				{
					fprintf(fDest32, "},\n");
					fprintf(fDest24, "},\n");
					fprintf(fDest18, "},\n");
					fprintf(fDest16, "},\n");
					fprintf(fDest8, "},\n");
					if(x == bih.biHeight)
					{
						fprintf(fDest32, "{");
						fprintf(fDest24, "{");
						fprintf(fDest18, "{");
						fprintf(fDest16, "{");
						fprintf(fDest8, "{");
					}
				}
				else
				{
					fprintf(fDest32, "\n");
					fprintf(fDest24, "\n");
					fprintf(fDest18, "\n");
					fprintf(fDest16, "\n");
					fprintf(fDest8, "\n");
				}

			}
 
		} 
	}
	//Print the closing string that makes this a variable
	if(dimensions == 2)
	{
		//fprintf(fDest, "}");
	}

	fprintf(fDest32, "};\n");
	fprintf(fDest24, "};\n");
	fprintf(fDest18, "};\n");
	fprintf(fDest16, "};\n");
	fprintf(fDest8, "};\n");

	fclose(fDest32);
	fclose(fDest24);
	fclose(fDest18);
	fclose(fDest16);
	fclose(fDest8);
	return 0;
}
void _bmpImage::downConvert8(const RGBTRIPLE *pixel, BYTE *newValue)
{
	// Assuming the desired color config is RRRGGGBB
	RGBTRIPLE newPixel = *pixel;
	//The ranges for R are [0-255] and [0-7]
	newPixel.rgbtRed = linearConvert(pixel->rgbtRed, 0, 255, 0, 7);
	//The ranges for G are [0-255] and [0-7]
	newPixel.rgbtGreen = linearConvert(pixel->rgbtGreen, 0, 255, 0, 7);
	//The ranges for B are [0-255] and [0-3]
	newPixel.rgbtBlue = linearConvert(pixel->rgbtBlue, 0, 255, 0, 3);

#if DEBUG == 1
	printf("After Linear Conversion:\n\tR = %f\n\tG = %f\n\tB = %f\n", R, G, B);
#endif

	//Shift them back into place
	newPixel.rgbtRed   = (newPixel.rgbtRed   << 5) & 0xE0;
	newPixel.rgbtGreen = (newPixel.rgbtGreen << 2) & 0x1C;
	newPixel.rgbtBlue  = (newPixel.rgbtBlue  << 0) & 0x03;

	*newValue = newPixel.rgbtRed + newPixel.rgbtGreen + newPixel.rgbtBlue;

#if DEBUG == 1
	printf("\nDownscaled: %i\n", newValue);
#endif
}
void _bmpImage::downConvert16(const RGBTRIPLE *pixel, BYTE *newValue1, BYTE *newValue2)
{
	// Assuming the desired color config is RRRRRGGG GGGBBBBB
	RGBTRIPLE newPixel = *pixel;
	//The ranges for R are [0-255] and [0-31]
	newPixel.rgbtRed = linearConvert(pixel->rgbtRed, 0, 255, 0, 31);
	//The ranges for G are [0-255] and [0-63]
	newPixel.rgbtGreen = linearConvert(pixel->rgbtGreen, 0, 255, 0, 63);
	//The ranges for B are [0-3] and [0-31]
	newPixel.rgbtBlue = linearConvert(pixel->rgbtBlue, 0, 255, 0, 31);

#if DEBUG == 1
	printf("After Linear Conversion:\n\tR = %f\n\tG = %f\n\tB = %f\n", pixel->rgbtRed, pixel->rgbtGreen, pixel->rgbtBlue);
#endif

	*newValue1 = ((newPixel.rgbtRed   << 3) & 0xF8) | (newPixel.rgbtGreen & 0x07);
	*newValue2 = ((newPixel.rgbtGreen << 2) & 0xE0) | (newPixel.rgbtBlue  & 0x1F);
}
void _bmpImage::downConvert18(const RGBTRIPLE *pixel, RGBTRIPLE *newPixel)
{
	// Assuming the desired color config is RRRRRGGG GGGBBBBB
	//The ranges for R are [0-255] and [0-63]
	newPixel->rgbtRed = linearConvert(pixel->rgbtRed, 0, 255, 0, 63);
	//The ranges for G are [0-255] and [0-63]
	newPixel->rgbtGreen = linearConvert(pixel->rgbtGreen, 0, 255, 0, 63);
	//The ranges for B are [0-255] and [0-63]
	newPixel->rgbtBlue = linearConvert(pixel->rgbtBlue, 0, 255, 0, 63);

#if DEBUG == 1
	printf("After Linear Conversion:\n\tR = %f\n\tG = %f\n\tB = %f\n", pixel->rgbtRed, pixel->rgbtGreen, pixel->rgbtBlue);
#endif
}
RGBTRIPLE _bmpImage::getPixel(int x,int y)
{
	// Image define from earlier
	return image[(bih.biHeight-1-y)*bih.biWidth+x];
}
void _bmpImage::initBlank(int x, int y)
{
	DWORD imagesize = bih.biWidth*bih.biHeight; // Helps you allocate memory for the image
	image = new RGBTRIPLE[imagesize]; // Create a new image (I'm creating an array during runtime)
}
void _bmpImage::openSourceFile()
{
	HANDLE hfile;
	DWORD written;

	// Open the file
	hfile = CreateFile(sourceFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

	// Read the header
	ReadFile(hfile, &bfh, sizeof(bfh), &written, NULL);
	ReadFile(hfile, &bih, sizeof(bih), &written, NULL);

	// Read image
	DWORD imagesize = bih.biWidth*bih.biHeight; // Helps you allocate memory for the image
	image = new RGBTRIPLE[imagesize]; // Create a new image (I'm creating an array during runtime)
	ReadFile(hfile,image,imagesize*sizeof(RGBTRIPLE),&written,NULL); // Reads it off the disk
	// Close source file
	CloseHandle(hfile);
	
	// Now for some information
	if(bih.biBitCount == 32)
	{
		//Tell 'em how big it is
		std::cout << "The image bitCount is " << bih.biBitCount << ".\n";
	}
	else if(bih.biBitCount == 24)
	{
		//Tell 'em how big it is
		std::cout << "The image bitCount is " << bih.biBitCount << ". Setting Alpha channel to 0xFF.\n"; // Will output the height of the bitmap
		alpha = 0xFF;
	}
	else
	{
		std::cout << "The image bitCount is " << bih.biBitCount << ". This application requires a 16- or 32-bit source image\n"; // Will output the height of the bitmap
	}

	std::cout << "The image width is " << bih.biWidth << "\n"; // Will output the width of the bitmap
	std::cout << "The image height is " << bih.biHeight << "\n"; // Will output the height of the bitmap
}
void _bmpImage::setPixel(int x,int y, RGBTRIPLE color)
{
	// Image define from earlier
	image[(bih.biHeight-1-y)*bih.biWidth+x] = color;
}
void _bmpImage::setSourceFile(char *path)
{

	//TODAY this can't be set manually
	imageString = "compass_color";

	char tempFile[200];
	strncpy(tempFile, path, 200);
	PathRemoveFileSpec(tempFile);
	imageDir = tempFile;
	imageDir += "\\";

	sourceFile = path;
}
