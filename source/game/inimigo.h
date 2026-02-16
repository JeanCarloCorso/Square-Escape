#ifndef INIMIGO_H
#define INIMIGO_H

#include <gba.h>
#include <stdbool.h>
#include "player.h"
#include "../game_constantes.h"

// ===============================
// Estrutura inimigo
// ===============================
typedef struct {
    int x;
    int y;
    bool ativo;
    int explodindo;
} Inimigo;

// ===============================
// Variáveis externas
// ===============================
extern Inimigo listaInimigos[MAX_INIMIGOS];
extern int score;

// ===============================
// Funções públicas
// ===============================
void inicializarInimigos();
void atualizarInimigos();
void desenhaInimigos();
void verificaColisaoTiros(Player* player);
void verificaColisoes(Player* player);

#endif
