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

}//namespace img
}//namespace ktftd
