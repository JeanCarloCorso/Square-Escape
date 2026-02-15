#include <gba.h>
#include <stdio.h>
#include "menu.h"

static int blinkCounter = 0;
static int showText = 1;

void menuInit()
{
    REG_DISPCNT = MODE_0 | BG0_ENABLE;

    consoleDemoInit();

    iprintf("\x1b[2J");

    iprintf("\x1b[4;1H===========================");
    iprintf("\x1b[5;1H                           ");
    iprintf("\x1b[6;1H S Q U A R E  E S C A P E  ");
    iprintf("\x1b[7;1H                           ");
    iprintf("\x1b[8;1H===========================");

    iprintf("\x1b[14;1HPressione START para iniciar!");
}

void menuUpdate(GameState* state)
{
    scanKeys();
    u16 keys = keysDown();

    blinkCounter++;

    if(blinkCounter > 30)
    {
        blinkCounter = 0;
        showText = !showText;

        if(showText)
            iprintf("\x1b[14;1HPressione START para iniciar!");
        else
            iprintf("\x1b[14;1H                             ");
    }

    if(keys & KEY_START)
    {
        *state = STATE_GAME;
    }
}
