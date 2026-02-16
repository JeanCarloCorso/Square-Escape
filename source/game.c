#include "game.h"
#include <gba.h>
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "barra_superior.h"
#include "barra_inferior.h"

// ===============================
// Dimensões
// ===============================
#define LARGURA_NAVE      24
#define ALTURA_NAVE       16

#define LARGURA_INIMIGO   12
#define ALTURA_INIMIGO    8

#define ALTURA_PLACAR 25
#define LIMITE_SUPERIOR_JOGO (ALTURA_PLACAR + 5)
#define LIMITE_INFERIOR_JOGO (SCREEN_HEIGHT - ALTURA_PLACAR - 5)

#define MAX_INIMIGOS 7

// ===============================
// Estrutura inimigo
// ===============================
typedef struct {
    int x;
    int y;
    bool ativo;
    int explodindo;
} Inimigo;

Inimigo listaInimigos[MAX_INIMIGOS];

// ===============================
// Variáveis globais
// ===============================
int playerX;
int playerY;
int score;
int vidas;

// ===============================
// Barras fixas
// ===============================
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

// ===============================
// Limites
// ===============================
void protegeLimites(){
    if(playerX < 0) playerX = 0;
    if(playerX > SCREEN_WIDTH - LARGURA_NAVE)
        playerX = SCREEN_WIDTH - LARGURA_NAVE;

    if(playerY < LIMITE_SUPERIOR_JOGO)
        playerY = LIMITE_SUPERIOR_JOGO;

    if(playerY > LIMITE_INFERIOR_JOGO - ALTURA_NAVE)
        playerY = LIMITE_INFERIOR_JOGO - ALTURA_NAVE;
}

// ===============================
// Player
// ===============================
void desenhaPlayer(){
    for(int y = 0; y < ALTURA_NAVE; y++) {
        for(int x = 0; x < LARGURA_NAVE; x++) {

            int drawX = playerX + x;
            int drawY = playerY + y;

            if((unsigned)drawX >= SCREEN_WIDTH ||
               (unsigned)drawY >= SCREEN_HEIGHT)
                continue;

            u16 cor = 0;
            bool temPixel = false;

            if ((x > 10 && x < 15) && (y > 5 && y < 9)) {
                cor = RGB5(0,0,31);
                temPixel = true;
            }
            else if ((x > 16) && (y > 4 && y < 11)) {
                cor = RGB5(25,25,25);
                temPixel = true;
            }
            else if ((x > 6 && x <= 16) && (y > 3 && y < 12)) {
                cor = RGB5(18,18,18);
                temPixel = true;
            }
            else if ((x > 2 && x < 14) &&
                    ((y > 1 && y <= 4) || (y >= 11 && y < 14))) {
                cor = RGB5(12,12,12);
                temPixel = true;
            }
            else if ((x <= 6) && (y > 6 && y < 9)) {
                cor = RGB5(31,10,0);
                temPixel = true;
            }

            if(temPixel)
                setPixel(drawX, drawY, cor);
        }
    }
}

// ===============================
// Inimigos
// ===============================
void inicializarInimigos() {

    for(int i = 0; i < MAX_INIMIGOS; i++) {

        listaInimigos[i].explodindo = 0;
        listaInimigos[i].x = SCREEN_WIDTH + (i * 40);

        int alturaUtil =
            LIMITE_INFERIOR_JOGO -
            LIMITE_SUPERIOR_JOGO -
            ALTURA_INIMIGO;

        listaInimigos[i].y =
            (rand() % alturaUtil) + LIMITE_SUPERIOR_JOGO;

        listaInimigos[i].ativo = true;
    }
}

void atualizarInimigos() {

    for(int i = 0; i < MAX_INIMIGOS; i++) {

        if(!listaInimigos[i].ativo)
            continue;

        // Se está explodindo
        if(listaInimigos[i].explodindo > 0) {

            listaInimigos[i].explodindo++;

            if(listaInimigos[i].explodindo > 15) { // duração explosão
                listaInimigos[i].explodindo = 0;

                listaInimigos[i].x = SCREEN_WIDTH;

                int alturaUtil =
                    LIMITE_INFERIOR_JOGO -
                    LIMITE_SUPERIOR_JOGO -
                    ALTURA_INIMIGO;

                listaInimigos[i].y =
                    (rand() % alturaUtil) + LIMITE_SUPERIOR_JOGO;
            }

            continue;
        }

        // Movimento normal
        listaInimigos[i].x--;

        if(listaInimigos[i].x + LARGURA_INIMIGO <= 0) {

            listaInimigos[i].x = SCREEN_WIDTH;

            int alturaUtil =
                LIMITE_INFERIOR_JOGO -
                LIMITE_SUPERIOR_JOGO -
                ALTURA_INIMIGO;

            listaInimigos[i].y =
                (rand() % alturaUtil) + LIMITE_SUPERIOR_JOGO;

            score += 10;
        }
    }
}

void desenhaExplosao(int baseX, int baseY, int frame) {

    static u16 cor1;
    static u16 cor2;

    cor1 = RGB5(31, 20, 0);  // laranja
    cor2 = RGB5(31, 31, 0);  // amarelo

    int raio = frame;  // cresce com o tempo

    for(int y = -raio; y <= raio; y++) {

        int drawY = baseY + y;
        if(drawY < 0 || drawY >= SCREEN_HEIGHT) continue;

        int largura = raio - abs(y);

        int drawX = baseX - largura;

        if(drawX < 0) drawX = 0;
        if(drawX + largura*2 >= SCREEN_WIDTH)
            largura = (SCREEN_WIDTH - drawX) / 2;

        if(largura > 0) {

            DMA3COPY(
                &cor1,
                &videoBuffer[drawY * SCREEN_WIDTH + drawX],
                largura*2 | DMA16 | DMA_SRC_FIXED
            );
        }
    }

    // núcleo brilhante
    if(raio > 2) {
        DMA3COPY(
            &cor2,
            &videoBuffer[baseY * SCREEN_WIDTH + baseX - 2],
            4 | DMA16 | DMA_SRC_FIXED
        );
    }
}

void desenhaInimigos() {

    static u16 corCorpo;
    static u16 corDetalhe;
    static u16 corCockpit;
    static u16 corMotor;

    corCorpo   = RGB5(0, 31, 0);    // verde principal
    corDetalhe = RGB5(0, 20, 0);    // sombra
    corCockpit = RGB5(0, 0, 31);    // azul
    corMotor   = RGB5(31, 15, 0);   // laranja

    for(int i = 0; i < MAX_INIMIGOS; i++) {

        if(!listaInimigos[i].ativo) continue;

        int baseX = listaInimigos[i].x;
        int baseY = listaInimigos[i].y;

        for(int y = 0; y < ALTURA_INIMIGO; y++) {

            int drawY = baseY + y;
            if(drawY < 0 || drawY >= SCREEN_HEIGHT) continue;

            int inicio = 0;
            int largura = 0;

            if(listaInimigos[i].explodindo > 0) {
                desenhaExplosao(
                    baseX + LARGURA_INIMIGO/2,
                    baseY + ALTURA_INIMIGO/2,
                    listaInimigos[i].explodindo
                );
                continue;
            }

            // =========================
            // Formato base
            // =========================
            switch(y) {
                case 0:
                case 7:
                    inicio = 5; largura = 2; break;

                case 1:
                case 6:
                    inicio = 4; largura = 4; break;

                case 2:
                case 5:
                    inicio = 2; largura = 8; break;

                case 3:
                case 4:
                    inicio = 0; largura = 12; break;
            }

            int drawX = baseX + inicio;
            int larguraFinal = largura;

            // Clipping horizontal
            if(drawX < 0) {
                larguraFinal += drawX;
                drawX = 0;
            }

            if(drawX + larguraFinal > SCREEN_WIDTH)
                larguraFinal = SCREEN_WIDTH - drawX;

            if(larguraFinal > 0) {
                DMA3COPY(
                    &corCorpo,
                    &videoBuffer[drawY * SCREEN_WIDTH + drawX],
                    larguraFinal | DMA16 | DMA_SRC_FIXED
                );
            }

            // =========================
            // Sombra inferior
            // =========================
            if(y == 4 || y == 5) {
                int sombraX = baseX + 3;
                int sombraL = 6;

                if(sombraX >= 0 &&
                   sombraX + sombraL <= SCREEN_WIDTH) {

                    DMA3COPY(
                        &corDetalhe,
                        &videoBuffer[drawY * SCREEN_WIDTH + sombraX],
                        sombraL | DMA16 | DMA_SRC_FIXED
                    );
                }
            }

            // =========================
            // Cockpit
            // =========================
            if(y == 3) {
                int cockpitX = baseX + 3;
                int cockpitL = 3;

                if(cockpitX >= 0 &&
                   cockpitX + cockpitL <= SCREEN_WIDTH) {

                    DMA3COPY(
                        &corCockpit,
                        &videoBuffer[drawY * SCREEN_WIDTH + cockpitX],
                        cockpitL | DMA16 | DMA_SRC_FIXED
                    );
                }
            }

            // =========================
            // Motor traseiro
            // =========================
            if(y == 3 || y == 4) {
                int motorX = baseX + 10;

                if(motorX >= 0 && motorX < SCREEN_WIDTH) {

                    DMA3COPY(
                        &corMotor,
                        &videoBuffer[drawY * SCREEN_WIDTH + motorX],
                        2 | DMA16 | DMA_SRC_FIXED
                    );
                }
            }
        }
    }
}

// ===============================
// Colisão
// ===============================
void verificaColisoes() {

    for(int i = 0; i < MAX_INIMIGOS; i++) {

        if(!listaInimigos[i].ativo)
            continue;

        // 🔥 NÃO testa colisão se já está explodindo
        if(listaInimigos[i].explodindo > 0)
            continue;

        if(playerX < listaInimigos[i].x + LARGURA_INIMIGO &&
           playerX + LARGURA_NAVE > listaInimigos[i].x &&
           playerY < listaInimigos[i].y + ALTURA_INIMIGO &&
           playerY + ALTURA_NAVE > listaInimigos[i].y)
        {
            vidas--;

            listaInimigos[i].explodindo = 1;
            listaInimigos[i].x += 10;
        }
    }
}

// ===============================
// Init
// ===============================
void gameInit() {

    irqInit();
    irqEnable(IRQ_VBLANK);
    REG_DISPCNT = MODE_3 | BG2_ENABLE;

    playerX = 20;
    playerY = SCREEN_HEIGHT / 2;

    score = 0;
    vidas = 3;

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

    if(keys & KEY_LEFT)  playerX--;
    if(keys & KEY_RIGHT) playerX++;
    if(keys & KEY_UP)    playerY--;
    if(keys & KEY_DOWN)  playerY++;

    protegeLimites();

    atualizarInimigos();
    verificaColisoes();

    clearScreenArea(
        RGB5(0,0,0),
        LIMITE_SUPERIOR_JOGO,
        LIMITE_INFERIOR_JOGO
    );

    desenhaInimigos();
    desenhaPlayer();

    if(vidas <= 0)
        *state = STATE_GAMEOVER;
}
