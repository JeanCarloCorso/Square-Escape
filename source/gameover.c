#include <gba.h>
#include <stdio.h>
#include "gameover.h"

void gameOverInit()
{
    REG_DISPCNT = MODE_0 | BG0_ENABLE;

    consoleDemoInit();

    iprintf("\x1b[2J");

    iprintf("\x1b[2;1H===========================");
    iprintf("\x1b[3;1H                           ");
    iprintf("\x1b[4;1H     G A M E   O V E R     ");
    iprintf("\x1b[5;1H                           ");
    iprintf("\x1b[6;1H===========================");

    iprintf("\x1b[14;1HPressione START para voltar ao menu!");
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
