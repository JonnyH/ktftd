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

#include <cstdlib>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Useage: ./spk2png {file.spk} {palette}\n";
		return EXIT_FAILURE;
	}
	std::ifstream inSPK(argv[1]);
	std::ifstream inPalette(argv[2]);
	auto palImg = ktftp::img::LoadSPKImage(inSPK);
	auto palette = ktftp::img::LoadPalette(inPalette);
	auto img = palImg.getImage(palette);
	img.writePNG("test.png");
}
