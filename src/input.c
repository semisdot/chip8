#include <SDL2/SDL.h>

#include "input.h"

/* ---------------------------------------------------------------------------------------------------- */

static int keyboard_find_map(int key)
{
	int vkey = -1;
	int i;

	const uint8_t keyboard_map[CHIP8_KEYS] =
	{
		SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
		SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R,
		SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
		SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V
	};

	for (i = 0; i < CHIP8_KEYS; ++i)
	{
		if (keyboard_map[i] == key)
		{
			vkey = i;
		}
	}

	return vkey;
}

/* ---------------------------------------------------------------------------------------------------- */

void input(struct chip8 *chip8)
{
	char key;
	int vkey; // virtual key

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);

			case SDL_KEYDOWN:

				if (event.key.repeat == 0)
				{
					key = event.key.keysym.scancode;
					vkey = keyboard_find_map(key);

					if (vkey != -1)
					{
						chip8->keypad[vkey] = true;
					}
				}

				break;

			case SDL_KEYUP:

				if (event.key.repeat == 0)
				{
					key = event.key.keysym.scancode;
					vkey = keyboard_find_map(key);

					if (vkey != -1)
					{
						chip8->keypad[vkey] = false;
					}
				}

				break;
		}
	}
}
