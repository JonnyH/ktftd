#include <SDL.h>
#include <GLES2/gl2.h>
#include "scr.hpp"
#include "xcui.hpp"


bool quit = false;

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);


	auto *window = SDL_CreateWindow("KTFTD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);

	auto ctx = SDL_GL_CreateContext(window);

	ktftd::ui::UIManager interface(640, 480);

	std::ifstream backgroundImageStream("./TFD/GEOGRAPH/BACK01.SCR");
	std::ifstream palettesStream("./TFD/GEODATA/PALETTES.DAT");

	auto backgroundImagePaletted = ktftd::img::LoadSCRImage(backgroundImageStream);
	auto backgroundImagePalette = ktftd::img::LoadPalette(palettesStream, 0);

	auto backgroundImage = backgroundImagePaletted.getImage(backgroundImagePalette);

	auto backgroundWindow = std::shared_ptr<ktftd::ui::Window>(new ktftd::ui::Window(0, 0, 640, 480));

	backgroundWindow->setBackground(backgroundImage);

	interface.windows.insert(backgroundWindow);

	while (quit == false)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		interface.draw();

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(ctx);

	SDL_DestroyWindow(window);

	SDL_Quit();
}
