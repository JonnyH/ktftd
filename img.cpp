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
#include "scr.hpp"
#include "bdy.hpp"

#include <cstdlib>
#include <cassert>

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
		if (argc <= 2)
		{
			printUsage();
			return EXIT_SUCCESS;
		}

		std::string fileName(argv[2]);

		int spk = fileName.rfind(".SPK");
		int bdy = fileName.rfind(".BDY");
		int scr = fileName.rfind(".SCR");
		int dat = fileName.rfind(".DAT");
		int lbm = fileName.rfind(".LBM");

		bool requiresPalette;

		ktftd::img::Palette palette;
		ktftd::img::PaletteImage palettedImage;
		ktftd::img::Image outputImage;

		std::ifstream inFile(fileName);

		if (spk == fileName.length()-4)
		{
			std::cout << "Is SPK File\n";
			requiresPalette = true;
			palettedImage = ktftd::img::LoadSPKImage(inFile);
		}
		else if (bdy == fileName.length()-4)
		{
			std::cout << "Is BDY file\n";
			requiresPalette = true;
			palettedImage = ktftd::img::LoadBDYImage(inFile);
		}
		else if(scr == fileName.length()-4)
		{
			std::cout << "Is SCR file\n";
			requiresPalette = true;
			palettedImage = ktftd::img::LoadSCRImage(inFile);
		}
		else if(dat == fileName.length()-4)
		{
			std::cout << "Is DAT file\n";
			requiresPalette = true;
			//DAT files are the same as SCR
			palettedImage = ktftd::img::LoadSCRImage(inFile);
		}
		else if(lbm == fileName.length() - 4)
		{
			std::cout << "Is LBM file\n";
			requiresPalette = false;
			auto lbmFile = ktftd::img::LoadLBMImage(inFile);
			assert(lbmFile.palette);
			assert(lbmFile.image);
			outputImage = lbmFile.image->getImage(*lbmFile.palette);
		}
		else
		{
			std::cerr << "Unknown file type: \"" << fileName << "\"\n";
			printUsage();
			return EXIT_FAILURE;
		}

		if (requiresPalette)
		{
			if (argc <= 3)
			{
				printUsage();
				return EXIT_FAILURE;
			}
			std::string paletteName (argv[3]);
			int dat = paletteName.rfind(".DAT");
			int lbm = paletteName.rfind(".LBM");
			if (dat == paletteName.length() - 4)
			{
				std::cout << "Is dat palette\n";
				if (argc != 5)
				{
					printUsage();
					return EXIT_FAILURE;
				}
				else
				{
					int paletteOffset = atoi(argv[4]);
					std::ifstream paletteFile(paletteName);
					palette = ktftd::img::LoadPalette(paletteFile, paletteOffset);
				}
			}
			else if(lbm == paletteName.length()-4)
			{
				std::cout << "Is lbm palette\n";
				std::ifstream paletteFile(paletteName);
				auto lbmImage = ktftd::img::LoadLBMImage(paletteFile);
				palette = *lbmImage.palette;
			}
			else
			{
				std::cerr << "Unknown palette type: \"" << paletteName << "\"\n";
				printUsage();
				return EXIT_FAILURE;
			}
			outputImage = palettedImage.getImage(palette);
		}
		outputImage.writePNG(outputFileName);

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
		auto palette = ktftd::img::LoadPalette(inFile, paletteOffset);
		auto image = palette.toImage();
		image.writePNG(outputFileName);
	}
	else
	{
		printUsage();
		return EXIT_FAILURE;
	}
}
