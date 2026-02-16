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
static int sequenciaVitoria = 0;
static float velocidadeFinal = 0;

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

    sequenciaVitoria = 0;
    velocidadeFinal = 0;

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


    if(score >= MAX_SCORE && !sequenciaVitoria) {
        sequenciaVitoria = 1;
        velocidadeFinal = 2.0;
    }

    if(sequenciaVitoria) {

        velocidadeFinal += 0.2;
        player.x += velocidadeFinal;

        clearScreenArea(RGB5(0,0,0),
                        LIMITE_SUPERIOR_JOGO,
                        LIMITE_INFERIOR_JOGO);

        playerDesenha(&player);

        if(player.x > SCREEN_WIDTH) {
            sequenciaVitoria = 0;
            *state = STATE_VITORIA;
        }

        return;
    }

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
    inicializarFase();

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
