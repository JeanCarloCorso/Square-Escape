#include <gba_video.h>
#include <gba_dma.h>
#include "graphics.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

u16* videoBuffer = (u16*)0x6000000;

void setPixel(int x, int y, u16 color){
    if((unsigned)x < SCREEN_WIDTH &&
       (unsigned)y < SCREEN_HEIGHT){
        videoBuffer[y * SCREEN_WIDTH + x] = color;
    }
}

void clearScreen(u16 color){
    DMA3COPY(&color, videoBuffer,
             SCREEN_WIDTH * SCREEN_HEIGHT | DMA16 | DMA_SRC_FIXED);
}

void clearScreenArea(u16 color, int yStart, int yEnd)
{
    if(yStart < 0) yStart = 0;
    if(yEnd > SCREEN_HEIGHT) yEnd = SCREEN_HEIGHT;
    if(yStart >= yEnd) return;

    int linhas = yEnd - yStart;
    int totalPixels = linhas * SCREEN_WIDTH;

    DMA3COPY(
        &color,
        videoBuffer + (yStart * SCREEN_WIDTH),
        totalPixels | DMA16 | DMA_SRC_FIXED
    );
}