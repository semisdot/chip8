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

#define INSTRUCTIONS_PER_FRAME 700

/* ---------------------------------------------------------------------------------------------------- */

static void update_timers(struct chip8 *chip8);

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

	uint64_t start_frame_time, end_frame_time;
	double time_elapsed;
	int i;

	while (1)
	{
		SDL_SetRenderDrawColor(sdl.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(sdl.renderer);

		input(&chip8);

		start_frame_time = SDL_GetPerformanceCounter();

		for (i = 0; i < INSTRUCTIONS_PER_FRAME / 60; ++i)
		{
			chip8_instruction_exec(&chip8);
		}

		end_frame_time = SDL_GetPerformanceCounter();

		time_elapsed = (double)((end_frame_time - start_frame_time) / 1000) / SDL_GetPerformanceFrequency();

		SDL_SetRenderDrawColor(sdl.renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		draw(sdl.renderer, &chip8.display);

		update_timers(&chip8);

		SDL_RenderPresent(sdl.renderer);
		SDL_Delay(16.67f > time_elapsed ? 16.67f - time_elapsed : 0);
	}

	return 0;
}

/* ---------------------------------------------------------------------------------------------------- */

static void update_timers(struct chip8 *chip8)
{
	if (chip8->registers.DT)
	{
		chip8->registers.DT -= 1;
	}

	if (chip8->registers.ST)
	{
		chip8->registers.ST -= 1;

		// Playing sound
	}
	else
	{
		// Stop playing sound
	}
}
