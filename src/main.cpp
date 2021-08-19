#include <iostream>
#include <locale.h>
#include "game.h"
#include <ncurses.h>

int main(int arc, char * argv[]) {
    Game g;
    g.mainLoop();
}