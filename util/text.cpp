/*
 * =====================================================================================
 *
 *       Filename:  text.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/10/12 15:20:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "palette_image.hpp"
#include "scr.hpp"

#include <iostream>
#include <string>

void printUsage()
{
	std::cout << "Usage:\n"
	          << "./ktftdtext [font.dat] [{s,b}] [\"Your text\"] [palette.dat] [palette offset]\n";
}

int main(int argc, char **argv)
{
	if (argc <= 5)
	{
		printUsage();
		return EXIT_FAILURE;
	}
	std::ifstream fontFile(argv[1]);
	ktftd::img::FontSize size;
	switch (argv[2][0])
	{
		case 's':
			size = ktftd::img::FONTSIZE_SMALL;
			break;
		case 'b':
			size = ktftd::img::FONTSIZE_BIG;
			break;
		default:
			printUsage();
			return EXIT_FAILURE;
	}
	std::string text = std::string(argv[3]);
	std::ifstream paletteFile(argv[4]);
	int paletteOffset = atoi(argv[5]);
	auto font = ktftd::img::LoadFont(fontFile, size);
	auto palette = ktftd::img::LoadPalette(paletteFile, paletteOffset);
	auto img = font.RenderText(text);
	img.getImage(palette).writePNG("out.png");
}
