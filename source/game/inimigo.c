#include "inimigo.h"
#include <stdlib.h>
#include "../graphics.h"

// ===============================
// Variáveis globais
// ===============================
Inimigo listaInimigos[MAX_INIMIGOS];
int score;

// ===============================
// Explosões
// ===============================
static void desenhaExplosao(int baseX, int baseY, int frame) {
    static u16 cor1 = RGB5(31,20,0);
    static u16 cor2 = RGB5(31,31,0);

    int raio = frame;
    for(int y = -raio; y <= raio; y++) {
        int drawY = baseY + y;
        if(drawY < 0 || drawY >= SCREEN_HEIGHT) continue;

        int largura = raio - abs(y);
        int drawX = baseX - largura;
        if(drawX < 0) drawX = 0;
        if(drawX + largura*2 >= SCREEN_WIDTH)
            largura = (SCREEN_WIDTH - drawX) / 2;

        if(largura > 0) {
            DMA3COPY(&cor1, &videoBuffer[drawY * SCREEN_WIDTH + drawX],
                     largura*2 | DMA16 | DMA_SRC_FIXED);
        }
    }

    if(raio > 2) {
        DMA3COPY(&cor2, &videoBuffer[baseY * SCREEN_WIDTH + baseX - 2],
                 4 | DMA16 | DMA_SRC_FIXED);
    }
}

// ===============================
// Inicialização
// ===============================
void inicializarInimigos() {
    for(int i = 0; i < MAX_INIMIGOS; i++) {
        listaInimigos[i].explodindo = 0;
        listaInimigos[i].x = SCREEN_WIDTH + (i * 40);

        int alturaUtil = LIMITE_INFERIOR_JOGO - LIMITE_SUPERIOR_JOGO - ALTURA_INIMIGO;
        listaInimigos[i].y = (rand() % alturaUtil) + LIMITE_SUPERIOR_JOGO;

        listaInimigos[i].ativo = true;
    }
}

// ===============================
// Atualização
// ===============================
void atualizarInimigos() {
    for(int i = 0; i < MAX_INIMIGOS; i++) {
        if(!listaInimigos[i].ativo) continue;

        if(listaInimigos[i].explodindo > 0) {
            listaInimigos[i].explodindo++;
            if(listaInimigos[i].explodindo > 15) {
                listaInimigos[i].explodindo = 0;
                listaInimigos[i].x = SCREEN_WIDTH;
                int alturaUtil = LIMITE_INFERIOR_JOGO - LIMITE_SUPERIOR_JOGO - ALTURA_INIMIGO;
                listaInimigos[i].y = (rand() % alturaUtil) + LIMITE_SUPERIOR_JOGO;
            }
            continue;
        }

        listaInimigos[i].x--;
        if(listaInimigos[i].x + LARGURA_INIMIGO <= 0) {
            listaInimigos[i].x = SCREEN_WIDTH;
            int alturaUtil = LIMITE_INFERIOR_JOGO - LIMITE_SUPERIOR_JOGO - ALTURA_INIMIGO;
            listaInimigos[i].y = (rand() % alturaUtil) + LIMITE_SUPERIOR_JOGO;
        }
    }
}

// ===============================
// Desenho
// ===============================
void desenhaInimigos() {
    static u16 corCorpo   = RGB5(0,31,0);
    static u16 corDetalhe = RGB5(0,20,0);
    static u16 corCockpit = RGB5(0,0,31);
    static u16 corMotor   = RGB5(31,15,0);

    for(int i = 0; i < MAX_INIMIGOS; i++) {
        if(!listaInimigos[i].ativo) continue;

        int baseX = listaInimigos[i].x;
        int baseY = listaInimigos[i].y;

        for(int y = 0; y < ALTURA_INIMIGO; y++) {
            int drawY = baseY + y;
            if(drawY < 0 || drawY >= SCREEN_HEIGHT) continue;

            if(listaInimigos[i].explodindo > 0) {
                desenhaExplosao(baseX + LARGURA_INIMIGO/2,
                                baseY + ALTURA_INIMIGO/2,
                                listaInimigos[i].explodindo);
                continue;
            }

            int inicio=0, largura=0;
            switch(y) {
                case 0: case 7: inicio=5; largura=2; break;
                case 1: case 6: inicio=4; largura=4; break;
                case 2: case 5: inicio=2; largura=8; break;
                case 3: case 4: inicio=0; largura=12; break;
            }

            int drawX = baseX + inicio;
            int larguraFinal = largura;
            if(drawX < 0) { larguraFinal += drawX; drawX = 0; }
            if(drawX + larguraFinal > SCREEN_WIDTH) larguraFinal = SCREEN_WIDTH - drawX;

            if(larguraFinal > 0)
                DMA3COPY(&corCorpo, &videoBuffer[drawY * SCREEN_WIDTH + drawX],
                         larguraFinal | DMA16 | DMA_SRC_FIXED);

            if(y==4||y==5) {
                int sombraX = baseX + 3;
                if(sombraX >= 0 && sombraX + 6 <= SCREEN_WIDTH)
                    DMA3COPY(&corDetalhe, &videoBuffer[drawY*SCREEN_WIDTH + sombraX],
                             6 | DMA16 | DMA_SRC_FIXED);
            }

            if(y==3) {
                int cockpitX = baseX + 3;
                if(cockpitX >=0 && cockpitX+3 <= SCREEN_WIDTH)
                    DMA3COPY(&corCockpit, &videoBuffer[drawY*SCREEN_WIDTH + cockpitX],
                             3 | DMA16 | DMA_SRC_FIXED);
            }

            if(y==3||y==4) {
                int motorX = baseX + 10;
                if(motorX >= 0 && motorX < SCREEN_WIDTH)
                    DMA3COPY(&corMotor, &videoBuffer[drawY*SCREEN_WIDTH + motorX],
                             2 | DMA16 | DMA_SRC_FIXED);
            }
        }
    }
}

// ===============================
// Colisões
// ===============================
void verificaColisaoTiros(Player* player) {
    for(int i = 0; i < MAX_TIRO; i++){
        if(!player->tiros[i].ativo) continue;

        for(int j = 0; j < MAX_INIMIGOS; j++){
            if(!listaInimigos[j].ativo) continue;
            if(listaInimigos[j].explodindo > 0) continue;

            if(player->tiros[i].x + TIRO_LARGURA > listaInimigos[j].x &&
               player->tiros[i].x < listaInimigos[j].x + LARGURA_INIMIGO &&
               player->tiros[i].y + TIRO_ALTURA > listaInimigos[j].y &&
               player->tiros[i].y < listaInimigos[j].y + ALTURA_INIMIGO)
            {
                player->tiros[i].ativo = false;
                listaInimigos[j].explodindo = 1;
                score += PONTUACAO_INIMIGO;
                break;
            }
        }
    }
}

void verificaColisoes(Player* player) {
    for(int i = 0; i < MAX_INIMIGOS; i++) {
        if(!listaInimigos[i].ativo) continue;
        if(listaInimigos[i].explodindo > 0) continue;

        if(player->x < listaInimigos[i].x + LARGURA_INIMIGO &&
           player->x + LARGURA_NAVE > listaInimigos[i].x &&
           player->y < listaInimigos[i].y + ALTURA_INIMIGO &&
           player->y + ALTURA_NAVE > listaInimigos[i].y)
        {
            player->vidas--;
            listaInimigos[i].explodindo = 1;
            listaInimigos[i].x += 10;
        }
    }
}
