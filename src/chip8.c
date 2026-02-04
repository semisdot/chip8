#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "chip8.h"

/* ---------------------------------------------------------------------------------------------------- */

static size_t file_size(FILE *f);

/* ---------------------------------------------------------------------------------------------------- */

void init_chip8(struct chip8 *chip8)
{
	const uint8_t chip8_fonts[] =
	{
		/*
		0xF0, // 0b11110000, // ****
		0x90, // 0b10010000, // *  *
		0x90, // 0b10010000, // *  *
		0x90, // 0b10010000, // *  *
		0xF0, // 0b11110000, // ****
		*/

		0xF0, 0x90, 0x90, 0x90, 0xF0,	// 0
		0x20, 0x60, 0x20, 0x20, 0x70,	// 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0,	// 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0,	// 3
		0x90, 0x90, 0xF0, 0x10, 0x10,	// 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0,	// 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0,	// 6
		0xF0, 0x10, 0x20, 0x40, 0x40,	// 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0,	// 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0,	// 9
		0xF0, 0x90, 0xF0, 0x90, 0x90,	// A
		0xE0, 0x90, 0xE0, 0x90, 0xE0,	// B
		0xF0, 0x80, 0x80, 0x80, 0xF0,	// C
		0xE0, 0x90, 0x90, 0x90, 0xE0,	// D
		0xF0, 0x80, 0xF0, 0x80, 0xF0,	// E
		0xF0, 0x80, 0xF0, 0x80, 0x80 	// F
	};

	memset(chip8, 0, sizeof(*chip8));

	memcpy(&chip8->memory[CHIP8_FONT_LOAD_ADDRESS], chip8_fonts, sizeof(chip8_fonts));

	chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}

void load_chip8(struct chip8 *chip8, const char *rom_name)
{
	FILE *f;
	size_t rom_size;
	size_t max_size;

	f = fopen(rom_name, "rb");

	if (!f)
	{
		SDL_Log("ROM file \"%s\": %s\n", rom_name, strerror(errno));
		exit(1);
	}

	rom_size = file_size(f);
	max_size = sizeof(chip8->memory) - CHIP8_PROGRAM_LOAD_ADDRESS;

	if (rom_size > max_size)
	{
		SDL_Log("ROM size: %zu, Max size: %zu\n", rom_size, max_size);
		exit(1);
	}

	fread(&chip8->memory[CHIP8_PROGRAM_LOAD_ADDRESS], sizeof(chip8->memory[0]), rom_size, f);

	fclose(f);
}

/* ---------------------------------------------------------------------------------------------------- */

static size_t file_size(FILE *f)
{
	size_t size;

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	return size;
}
