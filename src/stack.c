#include <stdint.h>
#include <assert.h>

#include "stack.h"

/* ---------------------------------------------------------------------------------------------------- */

void stack_push(struct chip8 *chip8, uint16_t val)
{
	assert(chip8->registers.SP < CHIP8_STACK_SIZE); // check stack overflow

	chip8->stack[chip8->registers.SP++] = val;
}

unsigned short stack_pop(struct chip8 *chip8)
{
	assert(chip8->registers.SP > 0); // check stack underflow

	return chip8->stack[--chip8->registers.SP];
}
