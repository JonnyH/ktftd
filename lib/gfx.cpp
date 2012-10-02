/*
 * =====================================================================================
 *
 *       Filename:  gfx.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/10/12 17:42:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "gfx.hpp"

namespace ktftd
{
namespace gfx
{

Texture::~Texture()
{

}

Palette::~Palette()
{

}

static GFXDriver *g_driver = NULL;

GFXDriver&
GFXDriver::getGFXDriver()
{
	return *g_driver;
}

void
GFXDriver::setGFXDriver(GFXDriver *drv)
{
	g_driver = drv;
}

}//namespace gfx
}//namespace ktftd
