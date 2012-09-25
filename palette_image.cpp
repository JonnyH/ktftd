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

#include "palette_image.hpp"

#include <png.h>

namespace ktftp
{
namespace img
{
	Palette LoadPalette(std::istream &inStream)
	{
		return Palette();
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
