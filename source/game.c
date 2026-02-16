#include "game.h"
#include <gba.h>
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "barra_superior.h"
#include "barra_inferior.h"

// Dimensões do player
#define LARGURA_NAVE 24
#define ALTURA_NAVE  16
// Dimensões do inimigo
#define TAMANHO_SPRITE_ITEM 5

#define ALTURA_PLACAR 25
#define LIMITE_SUPERIOR_JOGO (ALTURA_PLACAR + 5)
#define LIMITE_INFERIOR_JOGO (SCREEN_HEIGHT - ALTURA_PLACAR - 5)

// Variáveis globais
int playerX;
int playerY;
int inimigoX;
int inimigoY;
int score;
int vidas;

void desenhaBarraSuperior(){
    DMA3COPY(
        barra_superiorBitmap,
        videoBuffer,
        SCREEN_WIDTH * ALTURA_PLACAR | DMA16
    );
}

void desenhaBarraInferior(){
    DMA3COPY(
        barra_inferiorBitmap,
        videoBuffer + SCREEN_WIDTH * (SCREEN_HEIGHT - ALTURA_PLACAR),
        SCREEN_WIDTH * ALTURA_PLACAR | DMA16
    );
}

void protegeLimites(){
    if(playerX < 0) playerX = 0;
    if(playerX > SCREEN_WIDTH - LARGURA_NAVE)
        playerX = SCREEN_WIDTH - LARGURA_NAVE;

    if(playerY < LIMITE_SUPERIOR_JOGO) playerY = LIMITE_SUPERIOR_JOGO;
    if(playerY > LIMITE_INFERIOR_JOGO - ALTURA_NAVE)
        playerY = LIMITE_INFERIOR_JOGO - ALTURA_NAVE;
}

void gerarNovoInimigo(){
    inimigoX = SCREEN_WIDTH - TAMANHO_SPRITE_ITEM;

    int alturaUtil = LIMITE_INFERIOR_JOGO - LIMITE_SUPERIOR_JOGO;
    int intervaloMaximo = alturaUtil - TAMANHO_SPRITE_ITEM;
    inimigoY = (rand() % intervaloMaximo) + LIMITE_SUPERIOR_JOGO;
}

void desenhaInimigo(){
    for(int y = inimigoY; y < inimigoY + TAMANHO_SPRITE_ITEM; y++)
        for(int x = inimigoX; x < inimigoX + TAMANHO_SPRITE_ITEM; x++)
            setPixel(x, y, RGB5(0,31,0));
}

void verificaColisao(){
    if(playerX < inimigoX + TAMANHO_SPRITE_ITEM &&
       playerX + LARGURA_NAVE > inimigoX &&
       playerY < inimigoY + TAMANHO_SPRITE_ITEM &&
       playerY + ALTURA_NAVE > inimigoY)
    {
        vidas--;
        gerarNovoInimigo();
    }
    else if(inimigoX + TAMANHO_SPRITE_ITEM <= 0) {
        score += 10;
        gerarNovoInimigo();
    }
}

void gameInit() {
    irqInit();
    irqEnable(IRQ_VBLANK);
    REG_DISPCNT = MODE_3 | BG2_ENABLE;
    
    playerX = 0;
    playerY = 0;

    score = 0;
    vidas = 3;

    desenhaBarraSuperior();
    desenhaBarraInferior();

    gerarNovoInimigo();
}

void desenhaPlayer() {
    protegeLimites();
    for(int y = 0; y < ALTURA_NAVE; y++) {
        for(int x = 0; x < LARGURA_NAVE; x++) {

            int drawX = playerX + x;
            int drawY = playerY + y;

            if ((unsigned)drawX >= SCREEN_WIDTH || (unsigned)drawY >= SCREEN_HEIGHT) {
                continue; 
            }

            u16 cor = 0;
            bool temPixel = false;

            // CABINE (Azul)
            if ((x > 10 && x < 15) && (y > 5 && y < 9)) {
                cor = RGB5(0, 0, 31);
                temPixel = true;
            }
            // BICO (Cinza claro)
            else if ((x > 16) && (y > 4 && y < 11)) {
                cor = RGB5(25, 25, 25);
                temPixel = true;
            }
            // CORPO PRINCIPAL (Cinza médio)
            else if ((x > 6 && x <= 16) && (y > 3 && y < 12)) {
                cor = RGB5(18, 18, 18);
                temPixel = true;
            }
            // ASAS (Cinza escuro)
            else if ((x > 2 && x < 14) && ((y > 1 && y <= 4) || (y >= 11 && y < 14))) {
                cor = RGB5(12, 12, 12);
                temPixel = true;
            }
            // PROPULSOR (Laranja/Fogo)
            else if ((x <= 6) && (y > 6 && y < 9)) {
                cor = RGB5(31, 10, 0);
                temPixel = true;
            }

            if (temPixel) {
                setPixel(drawX, drawY, cor);
            }
        }
    }
}


void gameUpdate(GameState* state) {
    VBlankIntrWait();

    scanKeys();
    u16 keys = keysHeld();
    
    if(keys & KEY_LEFT)  playerX--;
    if(keys & KEY_RIGHT) playerX++;
    if(keys & KEY_UP)    playerY--;
    if(keys & KEY_DOWN)  playerY++;

    protegeLimites();
    verificaColisao();

    clearScreenArea(RGB5(0,0,0), ALTURA_PLACAR, SCREEN_HEIGHT - ALTURA_PLACAR);

    inimigoX--;

    desenhaInimigo();
    desenhaPlayer();

    if(vidas <= 0) {
        *state = STATE_GAMEOVER;
    }
}
