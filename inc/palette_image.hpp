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
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <cstring>

namespace ktftd
{
namespace img
{
	//FIXME: Put Fontsize somewhere else?
	enum FontSize
	{
		FONTSIZE_SMALL,//8x9
		FONTSIZE_BIG//16x16
	};
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
		Image(){};
		Image(uint32_t sizeX, uint32_t sizeY) : sizeX(sizeX),sizeY(sizeY),data(new RGBAColor[sizeX*sizeY]){};
		uint32_t sizeX, sizeY;
		std::unique_ptr<RGBAColor[]> data;
		void writePNG(const char* fileName);
	};
	
	class Palette
	{
	public:
		Palette()	{}
		Palette(Image &image);
		Image toImage();
		
		RGBColor palette[256];
	};

	class PaletteImage
	{
	public:
		PaletteImage(){}
		PaletteImage(uint32_t sizeX, uint32_t sizeY) : sizeX(sizeX),sizeY(sizeY),data(new uint8_t[sizeX*sizeY])
		{
			//By default, in XCOM color idx 0 is transparent
			this->transparentValues.insert(0);
		}
		PaletteImage(uint32_t sizeX, uint32_t sizeY, uint8_t *data) : sizeX(sizeX),sizeY(sizeY),data(new uint8_t[sizeX*sizeY])
		{
			//By default, in XCOM color idx 0 is transparent
			this->transparentValues.insert(0);
			memcpy(this->data.get(), data, sizeX*sizeY);
		}

		uint32_t sizeX;
		uint32_t sizeY;
		std::unique_ptr<uint8_t[]> data;
		void setPixel(uint32_t x, uint32_t y, uint8_t val);
		uint8_t readPixel(uint32_t x, uint32_t y);
		std::set<uint8_t> transparentValues;

		Image getImage(Palette &palette);
	};

	class PaletteFont
	{
	public:
		PaletteFont(){}
		PaletteFont(uint32_t sizeX, uint32_t sizeY, uint32_t startASCII) : sizeX(sizeX),sizeY(sizeY),startASCII(startASCII),endASCII(startASCII)	{}
		uint32_t sizeX, sizeY, startASCII;
		uint32_t endASCII;
		std::vector<PaletteImage> characterImages;
		PaletteImage RenderText(std::string &text);
	};

	Palette LoadPalette(std::istream &inStream, int paletteNo);
}//namespace img
}//namespace ktftd

#endif //__PALETTE_IMAGE_HPP
