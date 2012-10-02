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

static bool PointIsWithinWindow(int pointX, int pointY, int winOffsetX, int winOffsetY, int winSizeX, int winSizeY)
{
	if ( (pointX >= winOffsetX && pointX < (winOffsetX + winSizeX))
		 &&(pointY >= winOffsetY && pointY < (winOffsetY + winSizeY)))
	{
		return true;
	}
	else
	{	
		return false;
	}
}

void
Window::setBackground(ktftd::img::PaletteImage &img, ktftd::img::Palette &palette)
{
	this->winTexture = ktftd::gfx::GFXDriver::getGFXDriver().createTexture(img);
	this->winPalette = ktftd::gfx::GFXDriver::getGFXDriver().createPalette(palette);
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
	ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(this->posX+5, this->posY+5, this->sizeX-10, this->sizeY-10, *this->winTexture.get(), *this->winPalette.get());

	if (this->child)
	{
		if (this->border)
			this->child->draw(this->posX+6, this->posY+6, this->sizeX-12, this->sizeY-12);
		else
			this->child->draw(this->posX, this->posY, this->sizeX, this->sizeY);
	}
}

void
Window::SendEvent(Event event)
{
	if (this->child)
	{
		if (this->border)
		{
			if (PointIsWithinWindow(event.mouseX, event.mouseY, this->posX+6, this->posY+6, this->sizeX-12, this->sizeY-12))
			{
				this->child->SendEvent(event);
			}
		}
		else
		{
			this->child->SendEvent(event);
		}
	}
}

void
Button::draw(int offsetX, int offsetY, int sizeX, int sizeY)
{
	if (this->pressed)
	{
		ktftd::img::RGBAColor color(188,188,188);
		ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(offsetX, offsetY, sizeX, sizeY, color);
		color = ktftd::img::RGBAColor(127,127,127);
		ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(offsetX+1, offsetY+1, sizeX-2, sizeY-2, color);
		color = ktftd::img::RGBAColor(100,100,100);
		ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(offsetX+2, offsetY+2, sizeX-4, sizeY-4, color);
	}
	else
	{
		ktftd::img::RGBAColor color(230,230,230);
		ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(offsetX, offsetY, sizeX, sizeY, color);
		color = ktftd::img::RGBAColor(188,188,188);
		ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(offsetX+1, offsetY+1, sizeX-2, sizeY-2, color);
		color = ktftd::img::RGBAColor(127,127,127);
		ktftd::gfx::GFXDriver::getGFXDriver().DrawRect(offsetX+2, offsetY+2, sizeX-4, sizeY-4, color);
	}

	this->text->draw(offsetX+3, offsetY+3, sizeX-6, sizeY-6);
}

void
Button::SendEvent(Event event)
{
	if (event.type == Event::EVENT_MOUSEDOWN)
	{
		this->pressed = true;
		if (this->mouseDownFn)
		{
			this->mouseDownFn();
		}
	}
	else if (event.type == Event::EVENT_MOUSEUP)
	{
		this->pressed = false;
		if (this->mouseUpFn)
		{
			this->mouseUpFn();
		}

	}
}

Button::Button(std::string text)
	: text(new Label(text))
{
	this->pressed = false;
}

void
Widget::SendEvent(Event event)
{
	//NULL event handler
	
}

Label::Label(std::string text, TextAlignment HAlign, TextAlignment VAlign)
	: HAlign(HAlign),VAlign(VAlign)
{
	this->setText(text);
}

void
Label::setText(std::string text)
{
	//TODO: Set label text
	this->text = text;
}

Widget::~Widget()
{

}

void
Label::draw(int offsetX, int offsetY, int sizeX, int sizeY)
{
//TODO: Draw label text
}

void
UIManager::draw()
{
	for (auto win : this->windows)
	{
		win->draw(this->sizeX, this->sizeY);
	}
	for (auto dialogue: this->dialogueStack)
	{
		dialogue->draw(this->sizeX, this->sizeY);
	}
}

void
UIManager::SendEvent(Event event)
{
	if (event.type == Event::EVENT_KEYDOWN)
	{
		if (this->keyboardFocus)
		{
			this->keyboardFocus->SendEvent(event);
		}
	}
	else
	{
		//Dialogues are always modal - only can send to top dialogue
		if (!dialogueStack.empty())
		{
			auto dialogue = dialogueStack.back();
			if (PointIsWithinWindow(event.mouseX, event.mouseY, dialogue->posX, dialogue->posY, dialogue->sizeX, dialogue->sizeY))
			{
				dialogue->SendEvent(event);
			}
		}
		else
		{
			for (auto win : this->windows)
			{
				if (PointIsWithinWindow(event.mouseX, event.mouseY, win->posX, win->posY, win->sizeX, win->sizeY))
				{
					win->SendEvent(event);
				}
			}
		}
	}
}


}//namespace ui
}//namespace ktftd

