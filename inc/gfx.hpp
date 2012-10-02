/*
 * =====================================================================================
 *
 *       Filename:  gfx.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/10/12 17:30:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __GFX_HPP
#define __GFX_HPP

#include "palette_image.hpp"

#include <memory>

namespace ktftd
{
namespace gfx
{

class Texture
{
public:
	virtual int getSizeX() = 0;
	virtual int getSizeY() = 0;
	virtual ~Texture();
};

class Palette
{
public:
	virtual ~Palette() = 0;
};

class GFXDriver
{
public:
	virtual std::shared_ptr<Texture> createTexture(ktftd::img::PaletteImage &img) = 0;
	virtual std::shared_ptr<Palette> createPalette(ktftd::img::Palette &palette) = 0;
	virtual void DrawRect(int posX, int posY, int sizeX, int sizeY, Texture &tex, Palette &palette, bool scale = false) = 0;
	virtual void DrawRect(int posX, int posY, int sizeX, int sizeY, ktftd::img::RGBAColor &color) = 0;

	static GFXDriver& getGFXDriver();
	static void setGFXDriver(GFXDriver *drv);

};

}//namespace gfx
}//namespace ktftd

#endif //__GFX_HPP
