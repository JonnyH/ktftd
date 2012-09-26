/*
 * =====================================================================================
 *
 *       Filename:  palette_image.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25/09/12 16:55:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <png.h>
#include <fstream>
#include <iostream>
#include <cassert>

#include "palette_image.hpp"



namespace ktftp
{
namespace img
{
	Palette LoadPalette(std::istream &inStream, int paletteNo)
	{
		Palette palette;

		//256 RGB values, with a 6 byte stub at the end (No idea what is it used for)
		inStream.seekg((256*3+6)*paletteNo, std::ios::beg);

		assert(inStream.good());

		inStream.read((char*)&palette.palette[0], 256*3);

		//The range is 0-63, should be 0-255, multiply all values by 4
		for (int i = 0; i < 256; i++)
		{
			palette.palette[i].r *= 4;
			palette.palette[i].g *= 4;
			palette.palette[i].b *= 4;
		}
		
		return palette;
	}

	Palette::Palette(Image &img)
	{
		assert(img.sizeX >= 256);
	}

	Image Palette::toImage()
	{
		Image image(256,1);

		for (int i = 0; i < 256; i++)
		{
			image.data[i] = RGBAColor(this->palette[i]);
		}

		return image;
	}

	void Image::writePNG(const char* fileName)
	{
		auto *fp = fopen(fileName, "wb");
		auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		auto info_ptr = png_create_info_struct(png_ptr);
		png_init_io(png_ptr, fp);
		png_set_IHDR(png_ptr, info_ptr, this->sizeX, this->sizeY, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		png_write_info(png_ptr, info_ptr);

		for (unsigned int y = 0; y < this->sizeY; y++)
		{
			png_write_row(png_ptr, (png_const_bytep)&this->data[y*this->sizeX]);
		}
		png_write_end(png_ptr, info_ptr);
		png_destroy_write_struct(&png_ptr, &info_ptr);

	}

	Image PaletteImage::getImage(Palette &palette)
	{
		Image img(this->sizeX, this->sizeY);
		size_t pos = 0;
		for (unsigned int y = 0; y < this->sizeY; y++)
		{
			for (unsigned int x = 0; x < this->sizeX; x++)
			{
				img.data[pos] = RGBAColor(palette.palette[this->data[pos]]);
				pos++;
			}
		}
		return img;
	}
}//namespace img
}//namespace ktftp
