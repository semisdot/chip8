#include <SDL2/SDL.h>

#include "draw.h"

/* ---------------------------------------------------------------------------------------------------- */

void draw(SDL_Renderer *renderer, struct chip8_display *display)
{
	int x, y;
	SDL_Rect pixel;

	pixel.w = CHIP8_PIXEL_SCALE;
	pixel.h = CHIP8_PIXEL_SCALE;

	for (y = 0; y < CHIP8_WINDOW_HEIGHT; ++y)
	{
		for (x = 0; x < CHIP8_WINDOW_WIDTH; ++x)
		{
			if (display->pixels[y][x])
			{
				pixel.x = x * CHIP8_PIXEL_SCALE;
				pixel.y = y * CHIP8_PIXEL_SCALE;

				SDL_RenderFillRect(renderer, &pixel);
			}
		}
	}
}
