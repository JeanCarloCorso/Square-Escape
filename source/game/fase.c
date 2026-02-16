#include "fase.h"

#define MAX_FASES 10

static int faseAtual = 1;
static int numeroInimigos = 1;

// ===============================
// Inicializa a fase
// ===============================
void inicializarFase() {
    faseAtual = 1;
    numeroInimigos = 1;
}

// ===============================
// Atualiza a fase de acordo com o score
// ===============================
void atualizarFase(int score) {
    int novaFase = score / 100 + 1;

    if(novaFase > MAX_FASES) novaFase = MAX_FASES;
    if(novaFase != faseAtual) {
        faseAtual = novaFase;
        numeroInimigos = faseAtual;
    }
}

// ===============================
// Getters
// ===============================
int getNumeroInimigos() {
    return numeroInimigos;
}

int getFaseAtual() {
    return faseAtual;
}
