#include <SDL2/SDL.h>

#include "input.h"

/* ---------------------------------------------------------------------------------------------------- */

static int keyboard_find_map(const uint8_t *keyboard_map, int key)
{
	int vkey = -1;
	int i;

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

void input(struct chip8 *chip8, const uint8_t *keyboard_map)
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
					vkey = keyboard_find_map(keyboard_map, key);

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
					vkey = keyboard_find_map(keyboard_map, key);

					if (vkey != -1)
					{
						chip8->keypad[vkey] = false;
					}
				}

				break;
		}
	}
}
