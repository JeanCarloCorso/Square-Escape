#include <gba.h>
#include <stdio.h>
#include <string.h>
#include "font.h"
#include "gameover.h"
#include "graphics.h"
#include "game_over.h"

extern int highScore;
extern int score;

// Função para desenhar texto simples (6x6 pixels por caractere)
void drawTextCentered(const char* texto, int y, u16 cor) {
    int larguraTotal = strlen(texto) * 6;
    int xStart = (SCREEN_WIDTH - larguraTotal) / 2;

    for(int i = 0; texto[i]; i++)
        drawChar(xStart + i*6, y, texto[i], cor);
}

void gameOverInit()
{
    REG_DISPCNT = MODE_3 | BG2_ENABLE;

    DMA3COPY(
        game_overBitmap,
        videoBuffer,
        SCREEN_WIDTH * SCREEN_HEIGHT | DMA16
    );

    if(score > highScore) {
        highScore = score;
    }

    // posição abaixo do centro da tela
    int yBase = SCREEN_HEIGHT / 2 + 20;

    // desenha a pontuação do jogador
    char playerScoreText[20];
    sprintf(playerScoreText, "SCORE: %d", score);
    drawTextCentered(playerScoreText, yBase, RGB5(31, 31, 31));

    // desenha o high score abaixo do player
    char highScoreText[25];
    sprintf(highScoreText, "HIGH SCORE: %d", highScore);
    drawTextCentered(highScoreText, yBase + 10, RGB5(31, 31, 31));
}

void gameOverUpdate(GameState* state)
{
    scanKeys();
    u16 keys = keysDown();

    if(keys & KEY_START)
    {
        *state = STATE_MENU;
    }
}
