#include <gba.h>
#include <stdio.h>
#include "gameover.h"
#include "graphics.h"
#include "game_over.h"

void gameOverInit()
{
    REG_DISPCNT = MODE_3 | BG2_ENABLE;

    DMA3COPY(
        game_overBitmap,
        videoBuffer,
        SCREEN_WIDTH * SCREEN_HEIGHT | DMA16
    );
}

void gameOverUpdate(GameState* state)
{
    scanKeys();
    u16 keys = keysDown();

    if(keys & KEY_START)
    {
        *state = STATE_MENU;
    }
}
