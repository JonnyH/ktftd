/*
 * =====================================================================================
 *
 *       Filename:  flc.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/09/12 17:09:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __FLC_HPP
#define __FLC_HPP

#include "vid.hpp"
#include <fstream>

namespace ktftd
{
namespace vid
{

Video *loadFLC(std::istream &inStream);

};//namespace vid
};//namespace ktftd
#endif //__FLC_HPP
