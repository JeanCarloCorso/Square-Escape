#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <gba_types.h>
#include <gba_video.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

extern u16* videoBuffer;

void setPixel(int x, int y, u16 color);
void clearScreen(u16 color);

#endif
