/*
    Copyright (C) 1999
    For non-commercial use only.

    File	: tga.cf
    Date	: 05/05/1999
    Author	: Nate Miller
    Contact	: vandals1@home.com

    Change Log
    **********
    7/26/99 - added code to fclose () a file when there is an error
    6/11/99 - added support for 8bit images, changed commenting
*/
#include "tga.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <gl/gl.h>

bool loadTGA(const char *filename, STGA& tgaFile)
{
	FILE *file;
	unsigned char type[4];
	unsigned char info[6];

        file = fopen(filename, "rb");

        if (!file)
		return false;

	fread (&type, sizeof (char), 3, file);
	fseek (file, 12, SEEK_SET);
	fread (&info, sizeof (char), 6, file);

	//image type either 2 (color) or 3 (greyscale)
	if (type[1] != 0 || (type[2] != 2 && type[2] != 3))
	{
		fclose(file);
		return false;
	}

	tgaFile.width = info[0] + info[1] * 256;
	tgaFile.height = info[2] + info[3] * 256;
	tgaFile.byteCount = info[4] / 8;

	if (tgaFile.byteCount != 3 && tgaFile.byteCount != 4) {
		fclose(file);
		return false;
	}

	long imageSize = tgaFile.width * tgaFile.height
 * tgaFile.width * tgaFile.byteCount;

	//allocate memory for image data
	tgaFile.data = new unsigned char[imageSize];

	//read in image data
	fread(tgaFile.data, sizeof(unsigned char), imageSize, file);

	//close file
	fclose(file);

	return true;
}