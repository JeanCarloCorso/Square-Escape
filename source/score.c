#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "score.h"
#include "game_constantes.h"
#include "graphics.h"
#include "font.h"
#include "inimigo.h"

int highScore = 0;

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

void desenhaHighScore() {
    if(score > highScore)
        highScore = score;

    const char* texto = "HIGH SCORE";
    char buf[6];
    sprintf(buf, "%d", highScore);

    int larguraTotal = strlen(texto)*6 + 1*6 + strlen(buf)*6;
    int alturaTotal = 6;
    int xStart = (SCREEN_WIDTH - larguraTotal) / 2;
    int yStart = SCREEN_HEIGHT - alturaTotal - 5;
    for(int y = yStart; y < yStart + alturaTotal; y++)
        for(int x = xStart; x < xStart + larguraTotal; x++)
            setPixel(x, y, COR_FUNDO);

    for(int i = 0; texto[i]; i++)
        drawChar(xStart + i*6, yStart, texto[i], COR_SCORE);

    int xValor = xStart + strlen(texto)*6 + 6;
    for(int i = 0; buf[i]; i++)
        drawChar(xValor + i*6, yStart, buf[i], COR_SCORE);
}