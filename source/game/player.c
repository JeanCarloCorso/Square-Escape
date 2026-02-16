#include "player.h"
#include "../graphics.h"
#include <string.h>
#include <stdio.h>
#include "../game_constantes.h"
#include "../font.h"
#include "../game_constantes.h"

static int vidasAnteriores = -1;

// ===============================
// Inicialização
// ===============================
void playerInit(Player* p, int startX, int startY, int vidas) {
    p->x = startX;
    p->y = startY;
    p->vidas = vidas;
    p->cooldownTiro = 0;

    for(int i = 0; i < MAX_TIRO; i++)
        p->tiros[i].ativo = false;
}

// ===============================
// Limites
// ===============================
void playerProtegeLimites(Player* p){
    if(p->x < 0) p->x = 0;
    if(p->x > SCREEN_WIDTH - LARGURA_NAVE) 
        p->x = SCREEN_WIDTH - LARGURA_NAVE;

    if(p->y < LIMITE_SUPERIOR_JOGO)
        p->y = LIMITE_SUPERIOR_JOGO;

    if(p->y > LIMITE_INFERIOR_JOGO - ALTURA_NAVE)
        p->y = LIMITE_INFERIOR_JOGO - ALTURA_NAVE;
}

// ===============================
// Tiros
// ===============================
void playerDisparaTiro(Player* p) {
    if(p->cooldownTiro > 0) return;

    for(int i = 0; i < MAX_TIRO; i++){
        if(!p->tiros[i].ativo){
            p->tiros[i].x = p->x + LARGURA_NAVE;
            p->tiros[i].y = p->y + ALTURA_NAVE / 2 - TIRO_ALTURA/2;
            p->tiros[i].ativo = true;
            p->cooldownTiro = COOLDOWN_TIRO;
            break;
        }
    }
}

void playerAtualizaTiros(Player* p) {
    if(p->cooldownTiro > 0) p->cooldownTiro--;

    for(int i = 0; i < MAX_TIRO; i++){
        if(!p->tiros[i].ativo) continue;

        p->tiros[i].x += VELOCIDADE_TIRO;

        if(p->tiros[i].x > SCREEN_WIDTH)
            p->tiros[i].ativo = false;
    }
}

// ===============================
// Desenho
// ===============================
void playerDesenha(Player* p){
    for(int y = 0; y < ALTURA_NAVE; y++) {
        for(int x = 0; x < LARGURA_NAVE; x++) {
            int drawX = p->x + x;
            int drawY = p->y + y;

            if((unsigned)drawX >= SCREEN_WIDTH ||
               (unsigned)drawY >= SCREEN_HEIGHT) continue;

            u16 cor = 0;
            bool temPixel = false;

            if ((x > 10 && x < 15) && (y > 5 && y < 9)) {
                cor = RGB5(0,0,31); temPixel = true;
            }
            else if ((x > 16) && (y > 4 && y < 11)) {
                cor = RGB5(25,25,25); temPixel = true;
            }
            else if ((x > 6 && x <= 16) && (y > 3 && y < 12)) {
                cor = RGB5(18,18,18); temPixel = true;
            }
            else if ((x > 2 && x < 14) &&
                     ((y > 1 && y <= 4) || (y >= 11 && y < 14))) {
                cor = RGB5(12,12,12); temPixel = true;
            }
            else if ((x <= 6) && (y > 6 && y < 9)) {
                cor = RGB5(31,10,0); temPixel = true;
            }

            if(temPixel) setPixel(drawX, drawY, cor);
        }
    }
}

void playerDesenhaTiros(Player* p){
    u16 corTiro = RGB5(31,0,0);

    for(int i = 0; i < MAX_TIRO; i++){
        if(!p->tiros[i].ativo) continue;

        for(int dy = 0; dy < TIRO_ALTURA; dy++){
            int drawY = p->tiros[i].y + dy;
            if(drawY < LIMITE_SUPERIOR_JOGO || drawY >= LIMITE_INFERIOR_JOGO) continue;

            for(int dx = 0; dx < TIRO_LARGURA; dx++){
                int drawX = p->tiros[i].x + dx;
                if(drawX >= 0 && drawX < SCREEN_WIDTH)
                    setPixel(drawX, drawY, corTiro);
            }
        }
    }
}

void playerDesenhaVidas(Player* p){
    if(p->vidas == vidasAnteriores && vidasAnteriores >= 0) return;
    vidasAnteriores = p->vidas;

    const char* texto = "VIDAS:";
    int xTexto = 2;
    int yTexto = 2;

    int larguraTexto = strlen(texto)*6;
    int larguraTotal = larguraTexto + 5*6 + 4*2;
    int alturaTotal = 6;

    for(int y = yTexto; y < yTexto + alturaTotal; y++){
        for(int x = xTexto; x < xTexto + larguraTotal; x++){
            setPixel(x, y, COR_FUNDO);
        }
    }

    for(int i = 0; texto[i]; i++)
        drawChar(xTexto + i*6, yTexto, texto[i], COR_SCORE);

    int margem = xTexto + larguraTexto + 4;
    int largura = 6;
    int altura = 6;
    int espacamento = 2;

    for(int i = 0; i < p->vidas; i++){
        int x0 = margem + i * (largura + espacamento);
        int y0 = yTexto;
        for(int y = 0; y < altura; y++){
            for(int x = 0; x < largura; x++){
                setPixel(x0 + x, y0 + y, COR_VIDA);
            }
        }
    }
}

void playerUpdate(Player* p, u16 keys) {
    // Movimentação
    if(keys & KEY_LEFT)  p->x--;
    if(keys & KEY_RIGHT) p->x++;
    if(keys & KEY_UP)    p->y--;
    if(keys & KEY_DOWN)  p->y++;

    // Disparo
    if(keys & KEY_A) playerDisparaTiro(p);

    // Limites do campo
    playerProtegeLimites(p);

    // Atualiza tiros
    playerAtualizaTiros(p);
}
