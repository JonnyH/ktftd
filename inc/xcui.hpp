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
#include <stack>
#include <functional>

namespace ktftd
{
namespace ui
{

class Widget
{
public:
	virtual void draw(int offsetX, int offsetY, int sizeX, int sizeY) = 0;
	virtual ~Widget() = 0;

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
	Label();
	void setText(std::string text);
	void setSize(ktftd::img::FontSize size);
	void setHAlign(TextAlignment alignment);
	void setVAlign(TextAlignment alignment);
	virtual void draw(int offsetX, int offsetY, int sizeX, int sizeY);
};

class Button : public Widget
{
public:
	Button(std::shared_ptr<Label> text);
	void setOnMouseDown(std::function<void ()> fn);
	void setOnMouseUp(std::function<void ()> fn);

	std::function<void()> mouseDownFn;
	std::function<void()> mouseUpFn;

	virtual void draw(int offsetX, int offsetY, int sizeX, int sizeY);
};

class Window
{
public:
	Window(int posX, int posY, int sizeX, int sizeY) : posX(posX),posY(posY),sizeX(sizeX),sizeY(sizeY),border(true){}
	void setBackground(ktftd::img::RGBAColor color);
	void setBackground(ktftd::img::Image &image);

	void draw(int screenSizeX, int screenSizeY);

	int posX, posY, sizeX, sizeY;
	bool border;

	std::shared_ptr<Widget> child;
	std::shared_ptr<ktftd::gfx::Texture> winTexture;
};

class Dialogue : public Window
{
public:
	Dialogue(int posX, int posY, int sizeX, int sizeY);


};

class UIManager
{
public:
	UIManager(int sizeX, int sizeY) : sizeX(sizeX),sizeY(sizeY){}
	std::set<std::shared_ptr<Window> > windows;
	std::stack<std::shared_ptr<Dialogue> > dialogueStack;
	void draw();

	int sizeX, sizeY;

};

}//namespace ui
}//namespace ktftd

#endif //__XCUI_HPP