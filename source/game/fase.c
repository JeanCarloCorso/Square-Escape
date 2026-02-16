#include "fase.h"
#include "../game_constantes.h"

extern int score;

static int faseAtual = 1;
static int numeroInimigos = 1;
static int jogoFinalizado = 0;

// ===============================
// Inicializa a fase
// ===============================
void inicializarFase() {
    faseAtual = 1;
    numeroInimigos = 1;
    jogoFinalizado = 0;
}

bool isJogoFinalizado() {
    if(score >= MAX_SCORE && !jogoFinalizado) {
        numeroInimigos = 0;
        jogoFinalizado = 1;
        return true;
    }

    if(jogoFinalizado) return true;
    return false;
}

// ===============================
// Atualiza a fase de acordo com o score
// ===============================
void atualizarFase(int score) {

    if(isJogoFinalizado()) return;

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

int jogoFoiFinalizado() {
    return jogoFinalizado;
}