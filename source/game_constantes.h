#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#include <gba.h>

// Dimensões
#define LARGURA_NAVE      24
#define ALTURA_NAVE       16

#define LARGURA_INIMIGO   12
#define ALTURA_INIMIGO    8

#define ALTURA_PLACAR 25
#define LIMITE_SUPERIOR_JOGO (ALTURA_PLACAR + 5)
#define LIMITE_INFERIOR_JOGO (SCREEN_HEIGHT - ALTURA_PLACAR - 5)

#define MAX_FASES 20
#define MAX_INIMIGOS MAX_FASES
#define MAX_TIRO 1

#define VELOCIDADE_TIRO 3
#define TIRO_LARGURA 3
#define TIRO_ALTURA 3
#define COOLDOWN_TIRO 20

#define COR_VIDA RGB5(31,0,0)   // vermelho
#define COR_SCORE RGB5(31,31,31) // branco
#define COR_FUNDO RGB5(0,0,0)    // preto

#endif
