#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "chip8.h"

/* ---------------------------------------------------------------------------------------------------- */

static long file_size(FILE *f);

/* ---------------------------------------------------------------------------------------------------- */

void init_chip8(struct chip8 *chip8)
{
	const uint8_t chip8_sprites[16 * 5] =
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

	memcpy(&chip8->memory[CHIP8_FONT_LOAD_ADDRESS], chip8_sprites, sizeof(chip8_sprites));

	chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}

void load_chip8(struct chip8 *chip8, const char *rom_name)
{
	FILE *f;
	long rom_size;
	long max_size;

	f = fopen(rom_name, "rb");

	if (!f)
	{
		fprintf(stderr, "ROM file \"%s\": %s\n", rom_name, strerror(errno));
		exit(1);
	}

	rom_size = file_size(f);
	max_size = sizeof(chip8->memory) - CHIP8_PROGRAM_LOAD_ADDRESS;

	if (rom_size > max_size) // assert(CHIP8_PROGRAM_LOAD_ADDRESS + rom_size < CHIP8_MEMORY_SIZE);
	{
		fprintf(stderr, "ROM size: %zu, Max size: %zu\n", rom_size, max_size);
		exit(1);
	}

	fread(&chip8->memory[CHIP8_PROGRAM_LOAD_ADDRESS], sizeof(chip8->memory[0]), rom_size, f);

	fclose(f);
}

/* ---------------------------------------------------------------------------------------------------- */

static long file_size(FILE *f)
{
	long size = -1;

	if (!f)
	{
		goto end;
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

end:
	return size;
}
