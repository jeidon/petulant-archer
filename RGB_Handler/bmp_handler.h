#ifndef __BMP_HANDLER_H__
#define __BMP_HANDLER_H__

//Loosely sourced from:
//http://www.dreamincode.net/forums/topic/26936-how-to-make-sense-of-the-bmp-format/

#include "shared.h"

class _bmpImage
{
private:
	//Private Properties
	//File components
	char		drive[_MAX_DRIVE];
	char		dir[_MAX_DIR];
	char		fname[_MAX_FNAME];
	char		ext[_MAX_EXT];
	std::string	workingDir;

	//Other stuff
	BYTE alpha;
	RGBTRIPLE	*image;
	std::string	sourceFile;

	//Private Methods
	void		downConvert8(const RGBTRIPLE *pixel, BYTE *newValue);
	void		downConvert16(const RGBTRIPLE *pixel, BYTE *newValue1, BYTE *newValue2);
	void		downConvert18(const RGBTRIPLE *pixel, RGBTRIPLE *newPixel);
public:

	//Public Methods
	int			createHeaderFile(int bpp, int dimensions);
	RGBTRIPLE	getPixel(int x, int y);
	void		initBlank(int x, int y);
	void		openSourceFile();
	void		setPixel(int x, int y, RGBTRIPLE color);
	void		setSourceFile(char *path);

	//Public Properties
	BITMAPFILEHEADER	bfh;
	BITMAPINFOHEADER	bih;

	//The following are part of the image pathing and filename stuff
//	std::string			imageString;
};

/*
// This is the definition as included in windows.h
// Added via comment only for reference
typedef struct tagBITMAPFILEHEADER { 
	WORD	bfType; 
	DWORD	bfSize; 
	WORD	bfReserved1; 
	WORD	bfReserved2; 
	DWORD	bfOffBits; 
} BITMAPFILEHEADER, *PBITMAPFILEHEADER; 

typedef struct tagBITMAPINFOHEADER {
	DWORD	biSize;				// size of structure, in bytes
	DWORD	biWidth;			// bitmap width, in pixels
	DWORD	biHeight;			// bitmap height, in pixels
	WORD	biPlanes;			// the number of color planes must be 1
	WORD	biBitCount;			// the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.
	DWORD	biCompression;
	DWORD	biSizeImage;
	DWORD	biXPelsPerMeter;
	DWORD	biYPelsPerMeter;
	DWORD	biClrUsed;
	DWORD	biClrImportant;
} BITMAPINFOHEADER;
*/

#endif __BMP_HANDLER_H__