#include "vitoria.h"
#include <gba.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "font.h"
#include "score.h"

extern int score;

void desenhaTextoCentralizado(int y, const char* texto, u16 cor) {
    int largura = strlen(texto) * 6;
    int x = (SCREEN_WIDTH - largura) / 2;

    for(int i = 0; texto[i]; i++) {
        drawChar(x + i*6, y, texto[i], cor);
    }
}

void vitoriaInit() {
    REG_DISPCNT = MODE_3 | BG2_ENABLE;
    clearScreen(RGB5(0,0,0));
}

void vitoriaUpdate(GameState* state) {

    VBlankIntrWait();
    scanKeys();
    u16 keys = keysDown();

    // Título
    desenhaTextoCentralizado(10, "VOCE VENCEU!", RGB5(31,31,0));

    // Nome do jogo
    desenhaTextoCentralizado(30, "SQUARE SCAPE", RGB5(0,31,31));

    // Texto história
    desenhaTextoCentralizado(50,  "DEPOIS DE ENFRENTAR ONDAS DE INIMIGOS", RGB5(31,31,31));
    desenhaTextoCentralizado(60,  "IMPLACAVEIS E DESAFIAR OS LIMITES", RGB5(31,31,31));
    desenhaTextoCentralizado(70, "DO ESPACO PROFUNDO A TRIPULACAO", RGB5(31,31,31));
    desenhaTextoCentralizado(80, "FINALMENTE PODE RETORNAR PARA CASA", RGB5(31,31,31));
    desenhaTextoCentralizado(90, "EM SEGURANCA", RGB5(31,31,31));
    desenhaTextoCentralizado(100, "A GALAXIA AGRADECE A SUA CORAGEM", RGB5(31,31,31));
    // Pontuação final
    char buffer[20];
    sprintf(buffer, "PONTUACAO FINAL: %d", score);
    desenhaTextoCentralizado(120, buffer, RGB5(31,31,31));

    // Instrução
    desenhaTextoCentralizado(140, "PRECIONE START PARA REINICIAR", RGB5(31,0,0));

    if(keys & KEY_START) {
        *state = STATE_MENU;
    }
}
