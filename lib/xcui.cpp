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
	if (this->border)
	{
		//FIXME: More efficient method of drawing border than 5*full-window quads
		ktftd::img::RGBAColor color(127, 127, 127);
		ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(this->posX, this->posY, this->sizeX, this->sizeY, color);
		color = ktftd::img::RGBAColor(188, 188, 188);
		ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(this->posX+1, this->posY+1, this->sizeX-2, this->sizeY-2, color);
		color = ktftd::img::RGBAColor(255, 255, 255);
		ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(this->posX+2, this->posY+2, this->sizeX-4, this->sizeY-4, color);
		color = ktftd::img::RGBAColor(188, 188, 188);
		ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(this->posX+3, this->posY+3, this->sizeX-6, this->sizeY-6, color);
		color = ktftd::img::RGBAColor(127, 127, 127);
		ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(this->posX+4, this->posY+4, this->sizeX-8, this->sizeY-8, color);

	}
	ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(this->posX+5, this->posY+5, this->sizeX-10, this->sizeY-10, *this->winTexture.get());
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

