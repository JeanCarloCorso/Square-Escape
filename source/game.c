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
#include "score.h"

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
    desenhaHighScore();

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
