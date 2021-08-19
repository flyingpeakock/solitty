#include "game.h"
#include <ncurses.h>

Game::Game() : win(sol) {
    win.print();
}

void Game::mainLoop() {
    bool playing = true;
    while (!sol.isWon() && playing) {
        wchar_t input = getch();
        switch (input) {
            case Keybinds::DOWN:
            case KEY_DOWN:
            down();
            break;
            case Keybinds::UP:
            case KEY_UP:
            up();
            break;
            case Keybinds::LEFT:
            case KEY_LEFT:
            left();
            break;
            case Keybinds::RIGHT:
            case KEY_RIGHT:
            right();
            break;
            case Keybinds::QUIT:
            playing = false;
            break;
            case Keybinds::SELECT:
            break;
        }
    }
}