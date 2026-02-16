#ifndef FONT_H
#define FONT_H

#include <gba.h>

// Função para desenhar um caractere
void drawChar(int x, int y, char c, u16 cor);

// Função para desenhar uma string
void drawString(int x, int y, const char* str, u16 cor);

// Função para desenhar uma string e limpar a área antes (útil para HUD)
void drawStringClear(int x, int y, const char* str, u16 cor, u16 bg);

#endif // FONT_H
