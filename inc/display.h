#include "chip8.h"

void display_clear(struct chip8_display *display);
bool display_draw_sprite(struct chip8_display *display, int x, int y, const uint8_t *sprite, int num);
