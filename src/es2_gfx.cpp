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
#include <string>

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

const std::string blitVertShaderSource = 
	"attribute vec2 a_position;\n" \
	"attribute vec2 a_texcoord;\n" \
	"varying vec2 v_texcoord;\n" \
	"void main()\n" \
	"{\n" \
	" v_texcoord = a_texcoord;\n" \
	" gl_Position = vec4(a_position.x,a_position.y,0,1);\n" \
	"}\n";

const std::string blitFragShaderSource =
	"uniform sampler2D u_sampler;\n" \
	"varying vec2 v_texcoord;\n" \
	"void main()\n" \
	"{\n" \
	" gl_FragColor = texture2D(u_sampler, v_texcoord);\n" \
	"}\n";


static GLuint CompileShader(GLenum shaderType, const std::string source)
{
	GLuint shader = glCreateShader(shaderType);
	const GLchar* glslString = source.c_str();
	GLint glslLength = source.length();
	glShaderSource(shader, 1, &glslString, &glslLength);
	glCompileShader(shader);

	GLint compileStatus = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		GLsizei logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::cerr << "Failed to compile shader. Log Length:" << logLength;
		char* logString;

		logString = new char[logLength+1];
		logString[0] = '\0';
		std::cerr << logLength << "\n\"\"\"\n";

		glGetShaderInfoLog(shader, logLength, NULL, logString);

		std::cerr << logString << "\n\"\"\"\n";

		delete[] logString;
	}

	return shader;
}

static GLuint LinkProgram(GLuint vertShader, GLuint fragShader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);

	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::cerr << "Link failed. Log length: " << logLength << "\n\"\"\"\n";
		char* logString = new char[logLength+1];
		glGetProgramInfoLog(program, logLength, NULL, logString);
		std::cerr << logLength << "\n\"\"\"\n";
		delete[] logString;
	}

	return program;
}

ES2GFXDriver::ES2GFXDriver(SDL_GLContext ctx, int winSizeX, int winSizeY)
	: ctx(ctx),viewSizeX(winSizeX),viewSizeY(winSizeY)
{
	GLuint blitVertShader = CompileShader(GL_VERTEX_SHADER, blitVertShaderSource);

	GLuint blitFragShader = CompileShader(GL_FRAGMENT_SHADER, blitFragShaderSource);

	this->blitProgramInfo.program = LinkProgram(blitVertShader, blitFragShader);

	this->blitProgramInfo.samplerUniform = glGetUniformLocation(this->blitProgramInfo.program, "u_sampler");
	this->blitProgramInfo.texcoordAttrib = glGetAttribLocation(this->blitProgramInfo.program, "a_texcoord");
	this->blitProgramInfo.positionAttrib = glGetAttribLocation(this->blitProgramInfo.program, "a_position");

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return std::shared_ptr<ES2Texture>(tex);
}


void
ES2GFXDriver::DrawRect(int posX, int posY, int sizeX, int sizeY, Texture &tex)
{
	ES2Texture &es2Tex = static_cast<ES2Texture&>(tex);
	float x1, x2, y1, y2;

	x1 = (float)posX / (float)this->viewSizeX;
	x1 = x1 * 2.0f - 1.0f;
	x2 = (float)(posX+sizeX) / (float)this->viewSizeX;
	x2 = x2 * 2.0f - 1.0f;
	y1 = (1.0f)-(float)posY / (float)this->viewSizeY;
	y1 = y1 * 2.0f - 1.0f;
	y2 = (1.0f)-(float)(posY+sizeY) / (float)this->viewSizeY;
	y2 = y2 * 2.0f - 1.0f;

	float vertices[4][2] = 
	{
		{x2,y1},
		{x1,y1},
		{x2,y2},
		{x1,y2}
	};

	float texCoords[4][2] =
	{
		{1.0f,0.0f},
		{0.0f,0.0f},
		{1.0f,1.0f},
		{0.0f,1.0f}
	};

	glUseProgram(this->blitProgramInfo.program);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, es2Tex.glTexID);
	
	glEnableVertexAttribArray(this->blitProgramInfo.positionAttrib);
	glVertexAttribPointer(this->blitProgramInfo.positionAttrib, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0][0]);
	glEnableVertexAttribArray(this->blitProgramInfo.texcoordAttrib);
	glVertexAttribPointer(this->blitProgramInfo.texcoordAttrib, 2, GL_FLOAT, GL_FALSE, 0, &texCoords[0][0]);
	glUniform1i(this->blitProgramInfo.samplerUniform, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

}//namespace ES2
}//namespace gfx
}//namespace ktftd

