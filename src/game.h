#pragma once
#include "window.h"

class Game {
    public:
    Game();
    void mainLoop();
    private:
    Solitaire sol;
    std::vector<Solitaire> prev;
    Window win;
    void up();
    void down();
    void left();
    void right();
    void select();
    void focusLastInTab(const int tabIdx);
};