#ifndef PLAYER_H
#define PLAYER_H

#include <gba.h>
#include <stdbool.h>
#include "../game_constantes.h"

typedef struct {
    int x;
    int y;
    bool ativo;
} Tiro;

typedef struct {
    int x;
    int y;
    int vidas;
    Tiro tiros[MAX_TIRO];
    int cooldownTiro;
} Player;

// ===============================
// Inicialização
// ===============================
void playerInit(Player* p, int startX, int startY, int vidas);

// ===============================
// Atualização
// ===============================
void playerUpdate(Player* p, u16 keys);
void playerDisparaTiro(Player* p);
void playerAtualizaTiros(Player* p);
void playerProtegeLimites(Player* p);

// ===============================
// Desenho
// ===============================
void playerDesenha(Player* p);
void playerDesenhaTiros(Player* p);
void playerDesenhaVidas(Player* p);

#endif
