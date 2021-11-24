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
    void findAndFocusCard(const wchar_t input);
    void focusLastInTab(const int tabIdx);
    void top();
    bool playAgain();
    bool isCardValue(const wchar_t val) const;
};