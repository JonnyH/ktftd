/*
 * =====================================================================================
 *
 *       Filename:  scr.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/09/12 11:15:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __SCR_HPP
#define __SCR_HPP

#include <iostream>

#include "palette_image.hpp"

namespace ktftd
{
namespace img
{
	PaletteImage LoadSCRImage(std::istream &inStream);
}//namespace img
}//namespace ktftd

#endif //__SCR_HPP
