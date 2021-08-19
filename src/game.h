#pragma once
#include "window.h"

class Game {
    public:
    Game();
    void mainLoop();
    private:
    Solitaire sol;
    Window win;
    void up();
    void down();
    void left();
    void right();
};