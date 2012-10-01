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
	this->winTexture = ktftd::gfx::GFXDriver::getGFXDriver().createTexture(img);
}

void
Window::draw(int screenSizeX, int screenSizeY)
{
	ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(this->posX, this->posY, this->sizeX, this->sizeY, *this->winTexture.get());
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

