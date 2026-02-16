#include "game.h"
#include <gba.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"
#include "barra_superior.h"
#include "barra_inferior.h"
#include "font.h"
#include "game/player.h"
#include "game_constantes.h"
#include "game/inimigo.h"
#include "game/fase.h"

// ===============================
// Variáveis globais
// ===============================
Player player;

// ===============================
// Barras fixas
// ===============================
void desenhaBarraSuperior() {
    DMA3COPY(
        barra_superiorBitmap,
        videoBuffer,
        SCREEN_WIDTH * ALTURA_PLACAR | DMA16
    );
}

void desenhaBarraInferior() {
    DMA3COPY(
        barra_inferiorBitmap,
        videoBuffer + SCREEN_WIDTH * (SCREEN_HEIGHT - ALTURA_PLACAR),
        SCREEN_WIDTH * ALTURA_PLACAR | DMA16
    );
}

// ===============================
// Score
// ===============================
void desenhaScore() {
    static int scoreAnterior = -1;

    if(score == scoreAnterior && scoreAnterior >= 0) return;
    scoreAnterior = score;

    const char* texto = "SCORE:";
    char buf[6];
    sprintf(buf, "%d", score);

    int xStart = SCREEN_WIDTH - (strlen(texto)*6 + 5*6 + 2) - 2;
    int yStart = 2;

    int larguraTotal = strlen(texto)*6 + 5*6 + 2;
    int alturaTotal = 6;

    for(int y = yStart; y < yStart + alturaTotal; y++)
        for(int x = xStart; x < xStart + larguraTotal; x++)
            setPixel(x, y, COR_FUNDO);

    for(int i = 0; texto[i]; i++)
        drawChar(xStart + i*6, yStart, texto[i], COR_SCORE);

    int xValor = xStart + strlen(texto)*6 + 2;
    for(int i = 0; buf[i]; i++)
        drawChar(xValor + i*6, yStart, buf[i], COR_SCORE);
}

// ===============================
// Init
// ===============================
void gameInit() {
    irqInit();
    irqEnable(IRQ_VBLANK);
    REG_DISPCNT = MODE_3 | BG2_ENABLE;

    playerInit(&player, 20, SCREEN_HEIGHT/2, 3);
    score = 0;

    desenhaBarraSuperior();
    desenhaBarraInferior();

    inicializarInimigos();
}

// ===============================
// Update
// ===============================
void gameUpdate(GameState* state) {
    VBlankIntrWait();

    scanKeys();
    u16 keys = keysHeld();

    playerUpdate(&player, keys);
    playerProtegeLimites(&player);

    atualizarInimigos();
    playerAtualizaTiros(&player);

    verificaColisaoTiros(&player);
    verificaColisoes(&player);

    clearScreenArea(RGB5(0,0,0),
                    LIMITE_SUPERIOR_JOGO,
                    LIMITE_INFERIOR_JOGO);

    desenhaInimigos();
    playerDesenhaTiros(&player);
    playerDesenha(&player);

    playerDesenhaVidas(&player);
    desenhaScore();

    atualizarFase(score);

    for(int i = 0; i < MAX_INIMIGOS; i++) {
        if(i < getNumeroInimigos())
            listaInimigos[i].ativo = true;
        else
            listaInimigos[i].ativo = false;
    }

    if(player.vidas <= 0)
        *state = STATE_GAMEOVER;
}
