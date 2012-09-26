/*
 * =====================================================================================
 *
 *       Filename:  spk.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25/09/12 16:29:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <fstream>
#include <cassert>

#include "spk.hpp"
#include "lbm.hpp"

namespace ktftp
{
namespace img
{
PaletteImage LoadSPKImage(std::istream &inStream)
{
	PaletteImage img(320,200);
	size_t pos = 0;

	while (inStream.good())
	{
		uint16_t a;
		inStream.read((char*)&a, 2);
		switch (a)
		{
			case 0xffff:
				inStream.read((char*)&a, 2);
				pos += a*2;
				break;
			case 0xfffe:
				inStream.read((char*)&a, 2);
				inStream.read((char*)&img.data[pos], a*2);
				pos += a*2;
				break;
			case 0xfffd:
				return img;
				break;
			default:
				assert(0);
		}
	}

	return img;
}
}//namespace img
}//namespace ktftp

