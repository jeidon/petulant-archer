#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif
#include "bmp_handler.h"

int _bmpImage::createHeaderFile(int bpp)
{
	int arrayWidth;
	FILE *fDest;
	std::string destfile;
	
	destfile = imageDir;
	destfile += imageString;

	if(bpp == bit32)
	{
		destfile += "_32bit.h";
		arrayWidth = bih.biWidth*4;
	}
	else if(bpp == bit24)
	{
		destfile += "_24bit.h";
		arrayWidth = bih.biWidth*3;
	}
	else if(bpp == bit18)
	{
		destfile += "_18bit.h";
		arrayWidth = bih.biWidth*3;
	}
	else if(bpp == bit16)
	{
		destfile += "_16bit.h";
		arrayWidth = bih.biWidth*2;
	}
	else if(bpp == bit8)
	{
		destfile += "_8bit.h";
		arrayWidth = bih.biWidth;
	}
	
	if((fDest = fopen(destfile.c_str(), "wb")) == NULL)
	{
 		printf("The file %s cannot be created\n", destfile.c_str());
		exit(0);
	}

	fprintf(fDest, "PROGMEM const unsigned char %s_%ibit", imageString.c_str(), bpp);
	
	if(dimensions == 2)
	{
		fprintf(fDest, "[%i][%i]", bih.biHeight, arrayWidth);
	}
	fprintf(fDest, " = {\n");

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
				fprintf(fDest, "{");
			}

			if(bpp == bit32)
			{
				//32Bit Easy... done
				fprintf(fDest, "0x%02x, ", alpha);
				fprintf(fDest, "0x%02x, ", pixel.rgbtRed);
				fprintf(fDest, "0x%02x, ", pixel.rgbtGreen);
				fprintf(fDest, "0x%02x, ", pixel.rgbtBlue);
			}
			else if(bpp == bit24)
			{
				//24Bit
				fprintf(fDest, "0x%02x, ", pixel.rgbtRed);
				fprintf(fDest, "0x%02x, ", pixel.rgbtGreen);
				fprintf(fDest, "0x%02x, ", pixel.rgbtBlue);
			}
			else if(bpp == bit18)
			{
				//18Bit
				downConvert18(&pixel, &newPixel);
				fprintf(fDest, "0x%02x, ", newPixel.rgbtRed);
				fprintf(fDest, "0x%02x, ", newPixel.rgbtGreen);
				fprintf(fDest, "0x%02x, ", newPixel.rgbtBlue);
			}
			else if(bpp == bit16)
			{
				//16Bit
				downConvert16(&pixel, &msb, &lsb);
				fprintf(fDest, "0x%02x, ", msb);
				fprintf(fDest, "0x%02x, ", lsb);
			}
			else if(bpp == bit8)
			{
				//8 bit
				downConvert8(&pixel, &newValue);
				fprintf(fDest, "0x%02x, ", newValue);
			}

			if(++Line==0 || Line%bih.biWidth == 0)
			{
				if(dimensions == 2)
				{
					fprintf(fDest, "},\n");
					if(x == bih.biHeight)
					{
						fprintf(fDest, "{");
					}
				}
				else
				{
					fprintf(fDest, "\n");
				}
			}
		} 
	}

	//Print the closing string that makes this a variable
	fprintf(fDest, "};\n");

	//All finished
	fclose(fDest);
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
