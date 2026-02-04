#include <SDL2/SDL.h>

#include "sdl.h"
#include "chip8.h"

/* ---------------------------------------------------------------------------------------------------- */

#define SDL_INIT_FLAGS		SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER

#define SDL_WINDOW_TITLE	"CHIP-8 Interpreter"
#define SDL_WINDOW_X		SDL_WINDOWPOS_CENTERED
#define SDL_WINDOW_Y		SDL_WINDOWPOS_CENTERED
#define SDL_WINDOW_WIDTH	CHIP8_WINDOW_WIDTH	* CHIP8_PIXEL_SCALE
#define SDL_WINDOW_HEIGHT	CHIP8_WINDOW_HEIGHT	* CHIP8_PIXEL_SCALE
#define SDL_WINDOW_FLAGS	0

#define SDL_RENDERER_INDEX	-1
#define SDL_RENDERER_FLAGS	SDL_RENDERER_ACCELERATED

/* ---------------------------------------------------------------------------------------------------- */

void init_sdl(struct sdl *sdl)
{
	if (SDL_Init(SDL_INIT_FLAGS) != 0)
	{
		SDL_Log("Could not initialize SDL subsystems: %s\n", SDL_GetError());
		exit(1);
	}

	atexit(SDL_Quit);

	sdl->window = SDL_CreateWindow(SDL_WINDOW_TITLE,
								   SDL_WINDOW_X,
								   SDL_WINDOW_Y,
								   SDL_WINDOW_WIDTH,
								   SDL_WINDOW_HEIGHT,
								   SDL_WINDOW_FLAGS);

	if (!sdl->window)
	{
		SDL_Log("Could not create SDL window: %s\n", SDL_GetError());
		exit(1);
	}

	sdl->renderer = SDL_CreateRenderer(sdl->window,
									   SDL_RENDERER_INDEX,
									   SDL_RENDERER_FLAGS);

	if (!sdl->renderer)
	{
		SDL_Log("Could not create SDL renderer: %s\n", SDL_GetError());
		exit(1);
	}
}
