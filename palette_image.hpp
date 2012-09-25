/*
 * =====================================================================================
 *
 *       Filename:  palette_image.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25/09/12 16:30:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __PALETTE_IMAGE_HPP
#define __PALETTE_IMAGE_HPP

#include <cstdint>
#include <memory>

namespace ktftp
{
namespace img
{

	class RGBColor
	{
	public:
		RGBColor(uint8_t r=0, uint8_t g=0, uint8_t b=0):r(r),g(g),b(b){}
		uint8_t r,g,b;
	};
	
	class RGBAColor
	{
	public:
		RGBAColor(uint8_t r=0,uint8_t g=0, uint8_t b=0, uint8_t a=255):r(r),g(g),b(b),a(a){}
		RGBAColor(RGBColor &col):RGBAColor(col.r,col.g,col.b){}
		uint8_t r,g,b,a;
	};
	
	class Image
	{
	public:
		Image(uint32_t sizeX, uint32_t sizeY) : sizeX(sizeX),sizeY(sizeY),data(new RGBAColor[sizeX*sizeY]){};
		uint32_t sizeX, sizeY;
		std::unique_ptr<RGBAColor[]> data;
		void writePNG(const char* fileName);
	};
	
	class Palette
	{
	public:
		Palette()
		{
			for (int i = 0; i < 256; i++)
			{
				palette[255-i] = RGBColor(i,i,i);
			}
		}
		
		RGBColor palette[256];
	};

	class PaletteImage
	{
	public:
		PaletteImage(uint32_t sizeX, uint32_t sizeY) : sizeX(sizeX),sizeY(sizeY),data(new uint8_t[sizeX*sizeY])
		{
		}
		uint32_t sizeX;
		uint32_t sizeY;
		std::unique_ptr<uint8_t[]> data;
		void setPixel(uint32_t x, uint32_t y, uint8_t val);
		uint8_t readPixel(uint32_t x, uint32_t y);

		Image getImage(Palette &palette);
	};

	Palette LoadPalette(std::istream &inStream);
}//namespace img
}//namespace ktftp

#endif //__PALETTE_IMAGE_HPP
