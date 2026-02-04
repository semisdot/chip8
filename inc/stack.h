#include <stdint.h>

#include "chip8.h"

void stack_push(struct chip8 *chip8, uint16_t val);
unsigned short stack_pop(struct chip8 *chip8);
