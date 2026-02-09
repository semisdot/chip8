#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "display.h"

#include "instructions.h"

/* ---------------------------------------------------------------------------------------------------- */

static uint16_t memory_get_instruction(struct chip8 *chip8)
{
	uint8_t byte1, byte2;
	uint16_t instruction;

	byte1 = chip8->memory[chip8->registers.PC];
	byte2 = chip8->memory[chip8->registers.PC + 1];

	instruction = byte1 << 8 | byte2;

	return instruction;
}

/* ---------------------------------------------------------------------------------------------------- */

void chip8_instruction_exec(struct chip8 *chip8)
{
	uint16_t instruction;

	uint16_t nnn;
	uint8_t  n;
	uint8_t  x;
	uint8_t  y;
	uint8_t  kk;

	instruction = memory_get_instruction(chip8);
	printf("Address: 0x%04x, Instruction: 0x%04x Desc:\n", chip8->registers.PC, instruction);

	chip8->registers.PC += 2;

	nnn	=  instruction & 0x0fff;
	n	=  instruction & 0x000f;
	x	= (instruction & 0x0f00) >> 8; // instruction >> 8 & 0x000f
	y	= (instruction & 0x00f0) >> 4; // instruction >> 4 & 0x000f
	kk	=  instruction & 0x00ff;

	switch (instruction)
	{
		case 0x00e0: // 00E0 - CLS

			display_clear(&chip8->display);

			printf("Clear the display\n");
			break;

		case 0x00ee: // 00EE - RET

			chip8->registers.PC = stack_pop(chip8);

			printf("Return from a subroutine to address (0x%03x)\n", chip8->stack[chip8->registers.SP]);
			break;

		default:

			switch (instruction & 0xf000)
			{
				case 0x1000: // 1nnn - JP addr

					chip8->registers.PC = nnn;

					printf("Jump to location nnn (0x%03x)\n", nnn);
					break;

				case 0x2000: // 2nnn - CALL addr

					stack_push(chip8, chip8->registers.PC);
					chip8->registers.PC = nnn;

					printf("Call subroutine at nnn (0x%03x)\n", nnn);
					break;

				case 0x3000: // 3xkk - SE Vx, byte

					if (chip8->registers.V[x] == kk)
					{
						chip8->registers.PC += 2;
					}

					printf("Skip next instruction if V[0x%x] (0x%02x) == 0x%02x\n", x, chip8->registers.V[x], kk);
					break;

				case 0x4000: // 4xkk - SNE Vx, byte

					if (chip8->registers.V[x] != kk)
					{
						chip8->registers.PC += 2;
					}

					printf("Skip next instruction if V[0x%x] (0x%02x) != 0x%02x\n", x, chip8->registers.V[x], kk);
					break;

				case 0x5000: // 5xy0 - SE Vx, Vy

					if (chip8->registers.V[x] == chip8->registers.V[y])
					{
						chip8->registers.PC += 2;
					}

					printf("Skip next instruction if V[0x%x] (0x%02x) == V[0x%x] (0x%02x)\n", x, chip8->registers.V[x],
																							  y, chip8->registers.V[y]);
					break;

				case 0x6000: // 6xkk - LD Vx, byte

					chip8->registers.V[x] = kk;

					printf("Set V[0x%x] = kk (0x%02x)\n", x, kk);
					break;

				case 0x7000: // 7xkk - ADD Vx, byte

					chip8->registers.V[x] += kk;

					printf("Set V[0x%x] += kk (0x%02x)\n", x, kk);
					break;

				case 0x8000: // 8***

					switch (instruction & 0x000f) // 8xy*
					{
						case 0x00: // 8xy0 - LD Vx, Vy

							chip8->registers.V[x] = chip8->registers.V[y];

							printf("Set V[0x%x] = V[0x%x] (0x%02x)\n", x, y, chip8->registers.V[y]);
							break;

						case 0x01: // 8xy1 - OR Vx, Vy

							printf("Set V[0x%x] (0x%02x) |= V[0x%x] (0x%02x)\n", x, chip8->registers.V[x],
																				 y, chip8->registers.V[y]);

							chip8->registers.V[x] |= chip8->registers.V[y];
							break;

						case 0x02: // 8xy2 - AND Vx, Vy

							printf("Set V[0x%x] (0x%02x) &= V[0x%x] (0x%02x)\n", x, chip8->registers.V[x],
																				 y, chip8->registers.V[y]);

							chip8->registers.V[x] &= chip8->registers.V[y];
							break;

						case 0x03: // 8xy3 - XOR Vx, Vy

							printf("Set V[0x%x] (0x%02x) ^= V[0x%x] (0x%02x)\n", x, chip8->registers.V[x],
																				 y, chip8->registers.V[y]);

							chip8->registers.V[x] ^= chip8->registers.V[y];
							break;

						case 0x04: // 8xy4 - ADD Vx, Vy
							{
								uint16_t tmp;

								tmp = chip8->registers.V[x] + chip8->registers.V[y];
								printf("Set V[0x%x] (0x%02x) += V[0x%x] (0x%02x), set VF = carry\n", x, chip8->registers.V[x],
																									 y, chip8->registers.V[y]);

								chip8->registers.V[0xf] = tmp > 0xff; // 255
								chip8->registers.V[x] = tmp;
							}

							break;

						case 0x05: // 8xy5 - SUB Vx, Vy

							chip8->registers.V[0xf] = chip8->registers.V[x] > chip8->registers.V[y];

							printf("Set V[0x%x] (0x%02x) -= V[0x%x] (0x%02x), set VF = NOT borrow\n", x, chip8->registers.V[x],
																									  y, chip8->registers.V[y]);
							chip8->registers.V[x] -= chip8->registers.V[y];
							break;

						case 0x06: // 8xy6 - SHR Vx {, Vy}

							chip8->registers.V[0xf] = chip8->registers.V[x] & 0x01 /*0b00000001*/; // LSB
							printf("Set VF = V[0x%x] (0x%02x) SHR 1\n", x, chip8->registers.V[x]);

							chip8->registers.V[x] /= 2;
							break;

						case 0x07: // 8xy7 - SUBN Vx, Vy

							chip8->registers.V[0xf] = chip8->registers.V[y] > chip8->registers.V[x];
							printf("Set V[0x%x] = V[0x%x] (0x%02x) - V[0x%x] (0x%02x), set VF = NOT borrow\n", x, y, chip8->registers.V[y], x, chip8->registers.V[x]);

							chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
							break;

						case 0x0E: // 8xyE - SHL Vx {, Vy}

							chip8->registers.V[0xf] = chip8->registers.V[x] & 0x80 /*0b10000000*/; // MSB
							printf("Set VF = V[0x%x] (0x%02x) SHL 1\n", x, chip8->registers.V[x]);

							chip8->registers.V[x] *= 2;
							break;

						default:

							printf("Unimplemented or invalid instruction\n");
							break;
					}

					break; // case 0x8000

				case 0x9000: // 9xy0 - SNE Vx, Vy

					if (chip8->registers.V[x] != chip8->registers.V[y])
					{
						chip8->registers.PC += 2;
					}

					printf("Skip next instruction if Vx != Vy\n");
					break;

				case 0xa000: // Annn - LD I, addr

					chip8->registers.I = nnn;

					printf("Set I = nnn (0x%03x)\n", nnn);
					break;

				case 0xb000: // Bnnn - JP V0, addr

					chip8->registers.PC = nnn + chip8->registers.V[0x0];

					printf("Jump to location nnn + V0\n");
					break;

				case 0xc000: // Cxkk - RND Vx, byte // 0xC000 ?! UPPERCASE
					{
						int rand_byte = rand() % 256; // [0, 255]
						chip8->registers.V[x] = rand_byte & kk;

						printf("Set Vx = random byte & kk\n");
					}

					break;

				case 0xd000: // Dxyn - DRW Vx, Vy, nibble

					chip8->registers.V[0xf] = display_draw_sprite(&chip8->display,
											   					   chip8->registers.V[x],
											   					   chip8->registers.V[y],
											   					   &chip8->memory[chip8->registers.I],
											   					   n);

					printf("Display 0x%x-byte sprite starting at location I (0x%03x) at (V[0x%x] (0x%02x), V[0x%x] (0x%02x)), set VF = collision\n", n, chip8->registers.I, x, chip8->registers.V[x], y, chip8->registers.V[y]);

					// draw_flag = true // use ?
					break;

				case 0xe000: // E***

					switch (instruction & 0x00ff) // Ex**
					{
						case 0x9e: // Ex9E - SKP Vx

							if (chip8->keypad[chip8->registers.V[x]])
							{
								chip8->registers.PC += 2;
							}

							printf("Skip next instruction if key with the value of Vx is pressed\n");
							break;

						case 0xa1: // ExA1 - SKNP Vx

							if (!chip8->keypad[chip8->registers.V[x]])
							{
								chip8->registers.PC += 2;
							}

							printf("Skip next instruction if key with the value of Vx is not pressed\n");
							break;

						default:

							printf("Unimplemented or invalid instruction\n");
							break;
					}

					break; // case 0xe000

				case 0xf000: // F***

					switch (instruction & 0x00ff) // Fx**
					{
						case 0x07: // Fx07 - LD Vx, DT

							chip8->registers.V[x] = chip8->registers.DT;

							printf("Set Vx = delay timer value\n");
							break;

#if 0
						case 0x0a: // Fx0A - LD Vx, K

							// chip8->registers.V[x] = wait_for_key_press();

							printf("Wait for a key press, store the value of the key in Vx\n");
							break;
#endif

						case 0x15: // Fx15 - LD DT, Vx

							chip8->registers.DT = chip8->registers.V[x];

							printf("Set delay timer = Vx\n");
							break;

						case 0x18: // Fx18 - LD ST, Vx

							chip8->registers.ST = chip8->registers.V[x];

							printf("Set sound timer = Vx\n");
							break;

						case 0x1E: // Fx1E - ADD I, Vx

							chip8->registers.I += chip8->registers.V[x];

							printf("Set I += Vx\n");
							break;

						case 0x29: // Fx29 - LD F, Vx

							chip8->registers.I = chip8->registers.V[x] * 5 /*offset*/;

							printf("Set I = location of sprite for digit Vx\n");
							break;

						case 0x33: // Fx33 - LD B, Vx

							chip8->memory[chip8->registers.I]	  = chip8->registers.V[x] / 100;	 // hundreds digit
							chip8->memory[chip8->registers.I + 1] = chip8->registers.V[x] / 10 % 10; // tens digit
							chip8->memory[chip8->registers.I + 2] = chip8->registers.V[x] % 10;		 // ones digit

							printf("Store BCD representation of Vx in memory locations I, I + 1, and I + 2\n");
							break;

						case 0x55: // Fx55 - LD [I], Vx
							{
								int i;

								for (i = 0; i <= x; ++i)
								{
									chip8->memory[chip8->registers.I + i] = chip8->registers.V[i];
								}
							}

							printf("Store registers V0 through Vx in memory starting at location I\n");
							break;

						case 0x65: // Fx65 - LD Vx, [I]
							{
								int i;

								for (i = 0; i <= x; ++i)
								{
									chip8->registers.V[i] = chip8->memory[chip8->registers.I + i];
								}
							}

							printf("Read registers V0 through Vx from memory starting at location I\n");
							break;

						default:

							printf("Unimplemented or invalid instruction\n");
							break;
					}

					break; // case 0xf000

				default:
					printf("Unimplemented or invalid instruction\n");
					break;
			}

			break; // default:
	}
}
