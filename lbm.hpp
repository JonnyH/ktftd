/*
 * =====================================================================================
 *
 *       Filename:  lbm.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/09/12 09:51:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __LBM_HPP
#define __LBM_HPP

#include <iostream>
#include <memory>

#include "palette_image.hpp"

namespace ktftd
{
namespace img
{
	class LBMImage
	{
	public:
		std::unique_ptr<Palette> palette;
		std::unique_ptr<PaletteImage> image;
	};

	LBMImage LoadLBMImage(std::istream &inStream);

}//namespace img
}//namespace ktftd

#endif
