/*
 * =====================================================================================
 *
 *       Filename:  xcui.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/10/12 15:21:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __XCUI_HPP
#define __XCUI_HPP

#include "palette_image.hpp"
#include "gfx.hpp"

#include <set>
#include <list>
#include <functional>

namespace ktftd
{
namespace ui
{

class Event
{
public:
	enum EventType
	{
	EVENT_MOUSEDOWN,
	EVENT_MOUSEUP,
	EVENT_KEYDOWN,
	};

	EventType type;
	int keycode;
	int mouseX, mouseY;
};

class Widget
{
public:
	virtual void draw(int offsetX, int offsetY, int sizeX, int sizeY) = 0;
	virtual ~Widget();
	virtual void SendEvent(Event event);

};

class HSplit : public Widget
{
public:
	HSplit(int splitPixels) : splitPixels(splitPixels){}
	int splitPixels;
	std::shared_ptr<Widget> leftChild;
	std::shared_ptr<Widget> rightChild;
};

class VSplit : public Widget
{
public:
	VSplit(int splitPixels) : splitPixels(splitPixels){}
	int splitPixels;
	std::shared_ptr<Widget> topChild;
	std::shared_ptr<Widget> bottomChild;
	virtual void draw(int offsetX, int offsetY, int sizeX, int sizeY);
};

enum TextAlignment
{
	TEXTALIGN_LEFT,
	TEXTALIGN_CENTER,
	TEXTALIGN_RIGHT,
};

class Label : public Widget
{
public:
	Label(std::string text, TextAlignment HAlign = TEXTALIGN_CENTER, TextAlignment VAlign = TEXTALIGN_CENTER);
	void setText(std::string text);
	void setSize(ktftd::img::FontSize size);
	void setHAlign(TextAlignment alignment);
	void setVAlign(TextAlignment alignment);
	virtual void draw(int offsetX, int offsetY, int sizeX, int sizeY);
	TextAlignment HAlign, VAlign;
	std::string text;
};

class Button : public Widget
{
public:
	Button(std::string text);
	void setOnMouseDown(std::function<void ()> fn);
	void setOnMouseUp(std::function<void ()> fn);

	std::function<void()> mouseDownFn;
	std::function<void()> mouseUpFn;

	virtual void draw(int offsetX, int offsetY, int sizeX, int sizeY);
	virtual void SendEvent(Event event);
	std::shared_ptr<Label> text;
	bool pressed;
};

class Window
{
public:
	Window(int posX, int posY, int sizeX, int sizeY) : posX(posX),posY(posY),sizeX(sizeX),sizeY(sizeY),border(true),scale(false){}
	void setBackground(ktftd::img::RGBAColor color);
	void setBackground(ktftd::img::PaletteImage &image, ktftd::img::Palette &palette);

	void draw(int screenSizeX, int screenSizeY);

	void SendEvent(Event event);

	int posX, posY, sizeX, sizeY;
	bool border;
	bool scale;

	std::shared_ptr<Widget> child;
	std::shared_ptr<ktftd::gfx::Texture> winTexture;
	std::shared_ptr<ktftd::gfx::Palette> winPalette;
};

class Dialogue : public Window
{
public:
	Dialogue(int posX, int posY, int sizeX, int sizeY) : Window(posX, posY, sizeX, sizeY){};


};

class UIManager
{
public:
	UIManager(int sizeX, int sizeY) : sizeX(sizeX),sizeY(sizeY){}
	std::set<std::shared_ptr<Window> > windows;
	std::list<std::shared_ptr<Dialogue> > dialogueStack;
	void draw();
	void SendEvent(Event event);

	int sizeX, sizeY;
	std::shared_ptr<Widget> keyboardFocus;

};

}//namespace ui
}//namespace ktftd

#endif //__XCUI_HPP
