#include <SDL2/SDL.h>
#include <stdint.h>
#include <time.h>

#include "draw.h"
#include "sdl.h"
#include "chip8.h"
#include "instructions.h"
#include "input.h"

#include "display.h"

/* ---------------------------------------------------------------------------------------------------- */

int main(int argc, char **argv)
{
	const char *rom_name = argv[1];

	const uint8_t keyboard_map[CHIP8_KEYS] =
	{
		SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
		SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R,
		SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
		SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V
	};

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

		input(&chip8, keyboard_map);

		chip8_instruction_exec(&chip8);

		SDL_SetRenderDrawColor(sdl.renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		draw(sdl.renderer, &chip8.display);

		SDL_RenderPresent(sdl.renderer);
		SDL_Delay(16);
	}

	return 0;
}
