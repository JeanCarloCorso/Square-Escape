#include "game.h"
#include <gba.h>
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"

#define TAMANHO_SPRITE_PLAYER 10
#define TAMANHO_SPRITE_ITEM 5
#define MAX_PONTOS 5

// Variáveis globais
int playerX;
int playerY;
int itemX;
int itemY;
int pontuacao;

void limpaTela(){
    static u16 cor = RGB5(0,0,0);
    DMA3COPY(&cor, videoBuffer, (SCREEN_WIDTH * SCREEN_HEIGHT) | DMA16 | DMA_SRC_FIXED);
}

void protegeLimites()
{
    if(playerX < 0) playerX = 0;
    if(playerX > SCREEN_WIDTH - TAMANHO_SPRITE_PLAYER)
        playerX = SCREEN_WIDTH - TAMANHO_SPRITE_PLAYER;

    if(playerY < 0) playerY = 0;
    if(playerY > SCREEN_HEIGHT - TAMANHO_SPRITE_PLAYER)
        playerY = SCREEN_HEIGHT - TAMANHO_SPRITE_PLAYER;
}

void gerarNovoItem()
{
    itemX = rand() % (SCREEN_WIDTH - TAMANHO_SPRITE_ITEM);
    itemY = rand() % (SCREEN_HEIGHT - TAMANHO_SPRITE_ITEM);
}

void desenhaItem()
{
    for(int y = itemY; y < itemY + TAMANHO_SPRITE_ITEM; y++)
        for(int x = itemX; x < itemX + TAMANHO_SPRITE_ITEM; x++)
            setPixel(x, y, RGB5(0,31,0));
}

void verificaColisao()
{
    if(playerX < itemX + TAMANHO_SPRITE_ITEM &&
       playerX + TAMANHO_SPRITE_PLAYER > itemX &&
       playerY < itemY + TAMANHO_SPRITE_ITEM &&
       playerY + TAMANHO_SPRITE_PLAYER > itemY)
    {
        pontuacao--;
        gerarNovoItem();
    }
}

void gameInit() {
    irqInit();
    irqEnable(IRQ_VBLANK);
    REG_DISPCNT = MODE_3 | BG2_ENABLE;
    
    playerX = 100;
    playerY = 60;
    itemX = 50;
    itemY = 80;
    pontuacao = MAX_PONTOS;

    gerarNovoItem();
}

void desenhaPlacar(GameState* state) {
    int larguraBarra = 10;   // largura de cada barra
    int alturaBarra = 10;    // altura de cada barra
    int xStart = 5;          // posição inicial X
    int yStart = 5;          // posição inicial Y
    u16 corBarra = RGB5(0,31,0); // verde

    // Limpa a área do placar antes de desenhar
    for(int y = yStart; y < yStart + alturaBarra; y++) {
        for(int x = xStart; x < xStart + larguraBarra * MAX_PONTOS; x++) {
            setPixel(x, y, RGB5(0,0,0)); // fundo preto
        }
    }

    // Desenha as barras restantes
    for(int i = 0; i < pontuacao; i++) {
        for(int y = yStart; y < yStart + alturaBarra; y++) {
            for(int x = xStart + i*larguraBarra; x < xStart + (i+1)*larguraBarra; x++) {
                setPixel(x, y, corBarra);
            }
        }
    }

    // Se zerou, mostra GAME OVER
    if(pontuacao <= 0) {
        *state = STATE_GAMEOVER;
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

    limpaTela();

    desenhaItem();

    for(int y = playerY; y < playerY + TAMANHO_SPRITE_PLAYER; y++)
        for(int x = playerX; x < playerX + TAMANHO_SPRITE_PLAYER; x++)
            setPixel(x, y, RGB5(31,0,0));

    verificaColisao();
    desenhaPlacar(state);
}
