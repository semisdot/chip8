#ifndef CHIP8_H
#define CHIP8_H

/* ---------------------------------------------------------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* ---------------------------------------------------------------------------------------------------- */

#define CHIP8_WINDOW_WIDTH		64
#define CHIP8_WINDOW_HEIGHT		32

#define CHIP8_PIXEL_SCALE		10

// #define CHIP8_BACKGROUND_COLOR RGB_BLACK
// #define CHIP8_FOREGROUND_COLOR RGB_WHITE

#define CHIP8_MEMORY_SIZE	4096
#define CHIP8_STACK_SIZE	16
#define CHIP8_REGISTERS		16
#define CHIP8_KEYS			16


#define CHIP8_FONT_LOAD_ADDRESS		0x000 // 0
#define CHIP8_PROGRAM_LOAD_ADDRESS	0x200 // 512

/* ---------------------------------------------------------------------------------------------------- */

struct chip8_display
{
	bool pixels[CHIP8_WINDOW_HEIGHT][CHIP8_WINDOW_WIDTH];
};

struct chip8_registers
{
	uint8_t  V[CHIP8_REGISTERS];
	uint16_t I;  // memory address
	uint8_t  DT; // delay timer
	uint8_t  ST; // sound timer
	uint16_t PC; // program counter
	uint8_t  SP; // stack pointer
};

struct chip8
{
	uint8_t 				memory[CHIP8_MEMORY_SIZE];
	struct chip8_display	display;
	uint16_t				stack[CHIP8_STACK_SIZE];
	struct chip8_registers	registers;
	bool					keypad[CHIP8_KEYS];
};

/* ---------------------------------------------------------------------------------------------------- */

void init_chip8(struct chip8 *chip8);
void load_chip8(struct chip8 *chip8, const char *rom_name);

/* ---------------------------------------------------------------------------------------------------- */

#endif // CHIP8_H
