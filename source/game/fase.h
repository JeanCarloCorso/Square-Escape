#ifndef FASE_H
#define FASE_H

#include <gba.h>

// ===============================
// Funções públicas
// ===============================
void inicializarFase();
void atualizarFase(int score);
int getNumeroInimigos();
int getFaseAtual();

#endif
