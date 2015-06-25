#ifndef __SHARED_H__
#define __SHARED_H__

#include <Windows.h>
#include <iostream>
#include "Shlwapi.h"

enum{bit8, bit16, bit24, bit32};

BYTE linearConvert(const BYTE x, float A, float B, float C, float D);

#endif __SHARED_H__