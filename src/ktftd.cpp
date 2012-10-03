#include <SDL.h>
#include <GLES2/gl2.h>
#include "scr.hpp"
#include "xcui.hpp"
#include "es2_gfx.hpp"
#include "sdlaudio.hpp"



int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	bool quit = false;


	auto *window = SDL_CreateWindow("KTFTD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 400, SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);

	auto ctx = SDL_GL_CreateContext(window);

	auto *gfxDriver = new ktftd::gfx::ES2::ES2GFXDriver(ctx, 640, 400);
	ktftd::gfx::GFXDriver::setGFXDriver(gfxDriver);

	auto *audioDriver = new ktftd::audio::SDL::SDLAudioDriver();
	ktftd::audio::AudioDriver::setAudioDriver(audioDriver);


	ktftd::ui::UIManager interface(640, 400);

	std::ifstream backgroundImageStream("./TFD/GEOGRAPH/BACK05.SCR");
	std::ifstream background2ImageStream("./TFD/GEOGRAPH/BACK05.SCR");
	std::ifstream palettesStream("./TFD/GEODATA/PALETTES.DAT");

	auto backgroundImage= ktftd::img::LoadSCRImage(backgroundImageStream);
	auto backgroundImagePalette = ktftd::img::LoadPalette(palettesStream, 0);
	auto background2Image= ktftd::img::LoadSCRImage(background2ImageStream);


	auto backgroundWindow = std::shared_ptr<ktftd::ui::Window>(new ktftd::ui::Window(0, 0, 640, 400));

	backgroundWindow->setBackground(backgroundImage, backgroundImagePalette);

	interface.windows.insert(backgroundWindow);

	auto dialogue = std::shared_ptr<ktftd::ui::Dialogue>(new ktftd::ui::Dialogue(50, 50, 540, 300));
	dialogue->border = true;
	dialogue->setBackground(background2Image, backgroundImagePalette);

	auto button = std::shared_ptr<ktftd::ui::Button>(new ktftd::ui::Button("test"));
	dialogue->child = button;

	button->mouseUpFn = [&quit](){quit = true;};

	interface.dialogueStack.push_back(dialogue);

	while (quit == false)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		interface.draw();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					quit = true;
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					SDL_MouseButtonEvent mouseEvent = event.button;
					ktftd::ui::Event uiEvent;
					uiEvent.type = ktftd::ui::Event::EVENT_MOUSEDOWN;
					uiEvent.mouseX = mouseEvent.x;
					uiEvent.mouseY = mouseEvent.y;
					interface.SendEvent(uiEvent);
					break;
				}
				case SDL_MOUSEBUTTONUP:
				{
					SDL_MouseButtonEvent mouseEvent = event.button;
					ktftd::ui::Event uiEvent;
					uiEvent.type = ktftd::ui::Event::EVENT_MOUSEUP;
					uiEvent.mouseX = mouseEvent.x;
					uiEvent.mouseY = mouseEvent.y;
					interface.SendEvent(uiEvent);
					break;
				}
				case SDL_KEYDOWN:
				{
					SDL_KeyboardEvent keyEvent = event.key;
					ktftd::ui::Event uiEvent;
					uiEvent.type = ktftd::ui::Event::EVENT_KEYDOWN;
					uiEvent.keycode = keyEvent.keysym.sym;
					interface.SendEvent(uiEvent);
					break;
				}
				default:
				{
					//Ignore unknown input
				}
			}
		}

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(ctx);

	SDL_DestroyWindow(window);

	SDL_Quit();
}
