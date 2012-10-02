/*
 * =====================================================================================
 *
 *       Filename:  es2_gfx.hpp
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

#ifndef __ES2_GFX_HPP
#define __ES2_GFX_HPP

#include "gfx.hpp"
#include "palette_image.hpp"
#include <SDL.h>
#include <GLES2/gl2.h>

namespace ktftd
{
namespace gfx
{
namespace ES2
{

class ES2Texture : public Texture
{
public:
	virtual int getSizeX();
	virtual int getSizeY();
	virtual ~ES2Texture();
	int sizeX, sizeY;
	GLuint glTexID;
};

class ES2GFXDriver : public GFXDriver
{
public:
	ES2GFXDriver(SDL_GLContext ctx, int winSizeX, int winSizeY);
	virtual std::shared_ptr<Texture> createTexture(ktftd::img::Image &img);
	virtual void DrawRect(int posX, int posY, int sizeX, int sizeY, Texture &tex, bool scale = false);
	virtual void DrawRect(int posX, int posY, int sizeX, int sizeY, ktftd::img::RGBAColor &color);
private:
	SDL_GLContext ctx;
	int viewSizeX, viewSizeY;
	class
	{
	public:
		GLuint program;
		GLuint positionAttrib;
		GLuint texcoordAttrib;
		GLuint samplerUniform;
		GLuint scaleUniform;
	} blitTexProgramInfo;
	class
	{
	public:
		GLuint program;
		GLuint positionAttrib;
		GLuint colorUniform;
	} blitColorProgramInfo;
};

}//namespace ES2
}//namespace gfx
}//namespace ktftd

#endif //__ES2_GFX_HPP
