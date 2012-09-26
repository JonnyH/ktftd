/*
 * =====================================================================================
 *
 *       Filename:  img.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/09/12 10:06:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "palette_image.hpp"
#include "spk.hpp"
#include "lbm.hpp"

#include <cstdlib>

static const char* outputFileName = "out.png";

void printUsage()
{
	std::cout << "Useage:\n" <<
		"\tDump image:\n" <<
		"\t\ttftpimg i [file.{spk,bdy,scr,dat}] [palette.dat] [palette offset]\n" <<
		"\t\ttftpimg i [file.{spk,bdy,scr,dat}] [palette.lbm]\n" <<
		"\t\ttftpimg i [file.lbm]\n" <<
		"\tDump palette:\n" <<
		"\t\ttftpimg p [palette.dat] [palette.offset]\n";
}

int main(int argc, char **argv)
{
	if (argc <= 1)
	{
		printUsage();
		return EXIT_SUCCESS;
	}
	if (argv[1][0] == 'i')
	{

	}
	else if (argv[1][0] == 'p')
	{
		if (argc != 4)
		{
			printUsage();
			return EXIT_FAILURE;
		}
		std::ifstream inFile(argv[2]);
		int paletteOffset = atoi(argv[3]);
		auto palette = ktftp::img::LoadPalette(inFile, paletteOffset);
		auto image = palette.toImage();
		image.writePNG(outputFileName);
	}
	else
	{
		printUsage();
		return EXIT_FAILURE;
	}
}
