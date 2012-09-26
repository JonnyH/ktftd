/*
 * =====================================================================================
 *
 *       Filename:  spk.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25/09/12 16:29:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __SPK_HPP
#define __SPK_HPP

#include "palette_image.hpp"

namespace ktftd
{
namespace img
{

PaletteImage LoadSPKImage(std::istream &inFile);
}//namespace img
}//namespace ktftd
#endif
