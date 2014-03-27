/*
	Copyright (C) 1999
	For non-commercial use only.

  	File	: tga.h
	Date	: 05/05/1999
	Author	: Nate Miller
	Contact	: vandals1@home.com
*/
#ifndef _TGA_H_
#define _TGA_H_

struct STGA
{
        STGA()
        {
                data = (unsigned char*)0;
                width = 0;
                height = 0;
                byteCount = 0;
        }

	~STGA() { delete[] data; data = 0; }

	void destroy() { delete[] data; data = 0; }

        int width;
	int height;
        unsigned char byteCount;
        unsigned char* data;
};

bool loadTGA(const char *filename, STGA& tgaFile);

#endif
 