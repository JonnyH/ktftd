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

void printUsage()
{
	std::cout << "Useage:\n" <<
		"\tDump image:\n" <<
		"\t\ttftdimg i [file.{spk,bdy,scr,dat}] [palette.dat] [palette offset]\n" <<
		"\t\ttftdimg i [file.{spk,bdy,scr,dat}] [palette.lbm]\n" <<
		"\t\ttftdimg i [file.lbm]\n" <<
		"\tDump palette:\n" <<
		"\t\ttftdimg p [palette.dat] [palette.offset]\n" <<
		"\t\ttftdimg p [palette.lbm]\n";
}

int main(int argc, char **argv)
{
	if (argc <= 2)
	{
		printUsage();
		return EXIT_SUCCESS;
	}
	std::string fileName(argv[2]);
	std::string outputFileName = fileName.substr(0, fileName.length()-4);
	outputFileName.append(".png");
	if (argv[1][0] == 'i')
	{
		if (argc <= 2)
		{
			printUsage();
			return EXIT_SUCCESS;
		}


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
		outputImage.writePNG(outputFileName.c_str());

	}
	else if (argv[1][0] == 'p')
	{
		if (argc < 3)
		{
			printUsage();
			return EXIT_FAILURE;
		}

		std::string paletteName(argv[2]);
		ktftd::img::Palette palette;

		if (paletteName.rfind(".LBM") == paletteName.length()-4)
		{
			std::ifstream paletteFile(paletteName);
			auto lbmImage = ktftd::img::LoadLBMImage(paletteFile);
			palette = *lbmImage.palette;
		}
		else if (paletteName.rfind(".DAT") == paletteName.length()-4)
		{
	
			if (argc != 4)
			{
				printUsage();
				return EXIT_FAILURE;
			}
			std::ifstream inFile(argv[2]);
			int paletteOffset = atoi(argv[3]);
			palette = ktftd::img::LoadPalette(inFile, paletteOffset);
		}
		else
		{
			std::cerr << "Unknown palette file type\n";
			printUsage();
			return EXIT_FAILURE;
		}
		
		auto image = palette.toImage();
		image.writePNG(outputFileName.c_str());
	}
	else
	{
		printUsage();
		return EXIT_FAILURE;
	}
}
