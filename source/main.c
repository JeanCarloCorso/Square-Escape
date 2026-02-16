#include "gamestate.h"
#include "game.h"
#include "menu.h"
#include "gameover.h"
#include <gba.h>

int main(void) {
    GameState state = STATE_MENU;
    while(1) {
        switch(state) {
            case STATE_MENU:
                menuInit();
                while(state == STATE_MENU) {
                    menuUpdate(&state);
                }
                break;
            case STATE_GAME:
                gameInit();
                while(state == STATE_GAME) {
                    gameUpdate(&state);
                }
                break;
            case STATE_GAMEOVER:
                gameOverInit();
                while(state == STATE_GAMEOVER) {
                    gameOverUpdate(&state);
                }
                break;
        }
    }
    return 0;
}
