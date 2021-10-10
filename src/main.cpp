#include <locale.h>
#include "game.h"

int main(int arc, char * argv[]) {
    while (true) {
        Game g;
        if (!g.mainLoop()) {
            break;
        }
    }
}