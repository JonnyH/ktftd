/*
 * =====================================================================================
 *
 *       Filename:  bdy.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/09/12 11:20:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "bdy.hpp"

#include <stdint.h>

namespace ktftp
{
namespace img
{

	PaletteImage LoadBDYImage(std::istream &inStream)
	{
		PaletteImage image(320,200);
		size_t pos = 0;

		while (inStream.good() && pos < 320*200)
		{
			uint8_t a;
			inStream.read((char*)&a, 1);
			if (a >= 129)
			{
				int drawRun = 257-a;
				uint8_t pixel;
				inStream.read((char*)&pixel, 1);
				for (int i = 0; i < drawRun; i++)
				{
					image.data[pos++] = pixel;
				}
			}
			else
			{
				int drawRun = a+1;
				inStream.read((char*)&image.data[pos], drawRun);
				pos += drawRun;
			}
		}
		return image;
	}

}//namespace img
}//namespace ktftp
