/*
 * =====================================================================================
 *
 *       Filename:  scr.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/09/12 11:18:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "scr.hpp"
#include <cassert>

namespace ktftd
{
namespace img
{

	PaletteImage LoadSCRImage(std::istream &inStream)
	{
		PaletteImage image(320,200);

		inStream.read((char*)&image.data[0], 320*200);

		return image;
	}
	
	PaletteFont LoadFont(std::istream &inStream, FontSize size)
	{
		int sizeX, sizeY;
		switch(size)
		{
			case FONTSIZE_SMALL:
			{
				sizeX = 8;
				sizeY = 9;
				break;
			}
			case FONTSIZE_BIG:
			{
				sizeX = 16;
				sizeY = 16;
				break;
			}
			default:
			{
				assert(0);
			}
		}
		int initialASCII = 33;
		PaletteFont font(sizeX, sizeY, initialASCII);
		char* readBuffer = new char[font.sizeX*font.sizeY];
		while (inStream.good())
		{
			inStream.read(readBuffer, font.sizeX*font.sizeY);
			if (inStream.eof())
			{
				break;
			}
			else
			{
				int maxXUsed = 0;
				for (int y = 0; y < sizeY; y++)
				{
					for (int x = 0; x < sizeX; x++)
					{
						if (readBuffer[y*sizeX + x])
						{
							if (x >= maxXUsed)
								maxXUsed = x+1;
						}
					}
				}
				char *imageBuffer = new char[maxXUsed * font.sizeY];

				for (unsigned int y = 0; y < font.sizeY; y++)
				{
					memcpy(&imageBuffer[y * maxXUsed], &readBuffer[y*font.sizeX], maxXUsed);
				}

				font.characterImages.emplace_back(maxXUsed, font.sizeY, (uint8_t*)imageBuffer);
				font.endASCII++;

				delete[] imageBuffer;
			}

		}
		delete[] readBuffer;
		return font;
	}
	

}//namespace img
}//namespace ktftd
