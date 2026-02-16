#include <gba.h>
#include <stdio.h>
#include "menu.h"
#include "graphics.h"
#include "press_start.h"
#include "tela_inicial.h"

static int blinkCounter = 0;
static int showText = 1;

void desenhaMenu(){
    DMA3COPY(
        tela_inicialBitmap,
        videoBuffer,
        SCREEN_WIDTH * SCREEN_HEIGHT | DMA16
    );
}

void ControlaStart(bool show)
{
    int x = 62;
    int y = 94;

    const u16* pressBitmap = (const u16*)press_startBitmap;
    const u16* fundoBitmap = (const u16*)tela_inicialBitmap;

    for (int i = 0; i < 32; i++)
    {
        u16* destino = &videoBuffer[(y + i) * SCREEN_WIDTH + x];

        if (show)
        {
            const u16* origem = &pressBitmap[i * 116];

            DMA3COPY(
                origem,
                destino,
                116 | DMA16
            );
        }
        else
        {
            const u16* origem = &fundoBitmap[(y + i) * SCREEN_WIDTH + x];

            DMA3COPY(
                origem,
                destino,
                116 | DMA16
            );
        }
    }
}



void esperaVBlank()
{
    while(REG_VCOUNT >= 160);
    while(REG_VCOUNT < 160);
}

void menuInit()
{
    REG_DISPCNT = MODE_3 | BG2_ENABLE;
    desenhaMenu();
}

void menuUpdate(GameState* state)
{
    scanKeys();
    u16 keys = keysDown();

    blinkCounter++;

    if(blinkCounter > 6500)
    {
        blinkCounter = 0;
        showText = !showText;

        esperaVBlank();

        if(showText)
            ControlaStart(true);
        else
            ControlaStart(false);
    }

    if(keys & KEY_START)
    {
        *state = STATE_GAME;
    }
}
