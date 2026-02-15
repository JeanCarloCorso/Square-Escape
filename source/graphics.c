#include <gba_video.h>
#include <gba_dma.h>
#include "graphics.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

u16* videoBuffer = (u16*)0x6000000;

void setPixel(int x, int y, u16 color)
{
    videoBuffer[y * SCREEN_WIDTH + x] = color;
}

void clearScreen(u16 color)
{
    DMA3COPY(&color, videoBuffer,
             SCREEN_WIDTH * SCREEN_HEIGHT | DMA16 | DMA_SRC_FIXED);
}
