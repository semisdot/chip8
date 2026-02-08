#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "display.h"

/* ---------------------------------------------------------------------------------------------------- */

void display_clear(struct chip8_display *display)
{
	memset(display->pixels, 0, sizeof(display->pixels));
}

bool display_draw_sprite(struct chip8_display *display, int x, int y, const uint8_t *sprite, int num)
{
	int i, j;
	int sp_x, sp_y; // sprite pixel
	bool pixel_collision;

	pixel_collision = false;

	for (i = 0; i < num; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			if (sprite[i] & (0b10000000 >> j))
			{
				sp_x = (x + j) % CHIP8_WINDOW_WIDTH;
				sp_y = (y + i) % CHIP8_WINDOW_HEIGHT;

				if (display->pixels[sp_y][sp_x])
				{
					pixel_collision = true;
				}

				display->pixels[sp_y][sp_x] ^= true;
			}
		}
	}

	return pixel_collision;
}
