#include <SDL.h>
#include <GLES2/gl2.h>
#include "xcui.hpp"

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);

	auto *window = SDL_CreateWindow("KTFTD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);

	auto ctx = SDL_GL_CreateContext(window);

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(window);

	SDL_Delay(3000);

	SDL_GL_DeleteContext(ctx);

	SDL_DestroyWindow(window);

	SDL_Quit();
}
