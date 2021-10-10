#pragma once
#include "window.h"

class Game {
    public:
    Game();
    Game(Solitaire s);
    bool mainLoop();
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
    void top();
    bool playAgain();
};