/*
 * =====================================================================================
 *
 *       Filename:  bdy.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/09/12 11:15:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __BDY_HPP
#define __BDY_HPP

#include <iostream>

#include "palette_image.hpp"

namespace ktftd
{
namespace img
{
	PaletteImage LoadBDYImage(std::istream &inStream);
}//namespace img
}//namespace tftp

#endif //__BDY_HPP
