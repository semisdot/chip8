#include <SDL2/SDL.h>
#include <stdint.h>
#include <time.h>

#include "sdl.h"
#include "init.h"
#include "instructions.h"
#include "input.h"
#include "draw.h"

#include "display.h"

/* ---------------------------------------------------------------------------------------------------- */

int main(int argc, char **argv)
{
	const char *rom_name = argv[1];

	struct sdl sdl;
	struct chip8 chip8;

	(void)argc;

	srand(time(NULL));

	init_sdl(&sdl);
	init_chip8(&chip8);
	load_chip8(&chip8, rom_name);

	// SDL_SetRenderDrawColor(sdl.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	// SDL_RenderClear(sdl.renderer);

	while (1)
	{
		SDL_SetRenderDrawColor(sdl.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(sdl.renderer);

		input(&chip8);

		chip8_instruction_exec(&chip8);

		SDL_SetRenderDrawColor(sdl.renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		draw(sdl.renderer, &chip8.display);

		SDL_RenderPresent(sdl.renderer);
		SDL_Delay(16);
	}

	return 0;
}
