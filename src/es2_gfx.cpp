/*
 * =====================================================================================
 *
 *       Filename:  es2_gfx.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/10/12 17:45:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "es2_gfx.hpp"

namespace ktftd
{
namespace gfx
{
namespace ES2
{


int
ES2Texture::getSizeX()
{
	return this->sizeX;
}

int ES2Texture::getSizeY()
{
	return this->sizeY;
}

ES2Texture::~ES2Texture()
{

}

ES2GFXDriver::ES2GFXDriver(SDL_GLContext ctx, int winSizeX, int winSizeY)
	: ctx(ctx),viewSizeX(winSizeX),viewSizeY(winSizeY)
{

}

std::shared_ptr<Texture>
ES2GFXDriver::createTexture(ktftd::img::Image &img)
{
	ES2Texture *tex = new ES2Texture();

	tex->sizeX = img.sizeX;
	tex->sizeY = img.sizeY;

	glGenTextures(1, &tex->glTexID);
	glBindTexture(GL_TEXTURE_2D, tex->glTexID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->sizeX, tex->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)img.data.get());

	return std::shared_ptr<ES2Texture>(tex);
}


void
ES2GFXDriver::DrawRect(int posX, int posY, int sizeX, int sizeY, Texture &tex)
{
	float x1, x2, y1, y2;

	x1 = (float)posX / (float)this->viewSizeX;
	x2 = (float)(posX+sizeX) / (float)this->viewSizeX;
	y1 = (float)posY / (float)this->viewSizeY;
	y2 = (float)(posY+sizeY) / (float)this->viewSizeY;

	float vertices[4][2] = 
	{
		{x1,y1},
		{x1,y2},
		{x2,y2},
		{x2,y1}
	};

	float texCoords[4][2] =
	{
		{0.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,1.0f},
		{1.0f,0.0f}
	};
	//TODO: Bind program, set blend state, attribs, draw

}

}//namespace ES2
}//namespace gfx
}//namespace ktftd

