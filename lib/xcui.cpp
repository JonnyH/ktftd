/*
 * =====================================================================================
 *
 *       Filename:  xcui.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/10/12 17:12:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "xcui.hpp"
#include "gfx.hpp"

namespace ktftd
{
namespace ui
{

void
Window::setBackground(ktftd::img::Image &img)
{

}

void
Window::draw(int screenSizeX, int screenSizeY)
{

}

void
UIManager::draw()
{
	for (auto win : this->windows)
	{
		win->draw(this->sizeX, this->sizeY);
	}
}


}//namespace ui
}//namespace ktftd

