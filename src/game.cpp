#include "game.h"
#include <ncurses.h>

Game::Game() {
    win = Window(sol);
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
        win.print();
    }
}

void Game::focusLastInTab(int tabIdx) {
    Deck tab = sol.getTableaus()[tabIdx];
    int lastIndex = tab.size() - 1;
    win.focus({Stack::TABLEAU, (tabIdx * 100) + lastIndex});
}

void Game::up() {
    Position current = win.getFocus();
    switch (current.stack) {
        case Stack::DECK:
        // Going up from deck places you in first tab bottom card
        focusLastInTab(0);
        break;
        case Stack::BUILD:
        // Places you in current.index + 3 tab bottom card
        focusLastInTab(current.index + 3);
        break;
        case Stack::DISCARD:
        // Places you in 2nd tab bottom card
        focusLastInTab(1);
        break;
        case Stack::TABLEAU:
        // If uncovered cards above focus those
        // else tab 0 focus deck, tab 1-2 focus discard
        // tab 3-6 focus build - 3
        {
            Deck tab = sol.getTableaus()[current.index / 100];
            if (current.index % 100 != 0) {
                for (auto i = (current.index % 100) - 1; i >= 0; i--) {
                    if (tab[i].getFacing() == Facing::FRONT) {
                        win.focus({Stack::TABLEAU, current.index - 1});
                        return;
                    }
                }
            }

            int tabIdx = current.index / 100;
            switch (tabIdx) {
                case 0:
                win.focus({Stack::DECK, 0});
                break;
                case 1:
                case 2:
                win.focus({Stack::DISCARD, 0});
                break;
                default:
                win.focus({Stack::BUILD, tabIdx - 3});
                break;
            }
        }
        break;
    }
}

void Game::down() {

}

void Game::left() {

}

void Game::right() {

}