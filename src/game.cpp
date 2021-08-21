#include "game.h"
#include "Stopwatch.h"
#include <ncurses.h>

Game::Game() : win(&sol){
    win.print();
}

void Game::mainLoop() {
    bool playing = true;
    Stopwatch timer;
    timer.start();
    while (!sol.isWon() && playing) {
        win.print();
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
            select();
            break;
            case Keybinds::UNDO:
            if (prev.size() == 0) {
                break;
            }
            sol = prev.back();
            prev.pop_back();
            break;
        }
    }
    timer.stop();
    if (sol.isWon()) {
        win.print();
        win.printMessage(timer.timeTaken());
        getch();
    }
}

void Game::select() {
    Position current = win.getFocus(); // from
    Position selected = win.getSelect(); // to

    if (current.stack == Stack::DECK) {
        sol.placeDiscard();
        return;
    }

    if (selected.stack == Stack::NONE) {
        win.select(current);
        return;
    }
    else {
        prev.push_back(sol);
    }

    if (current.stack == selected.stack && current.index == selected.index) {
        win.select({Stack::NONE, 0});
        return;
    }

    if (current.stack == Stack::TABLEAU && selected.stack == Stack::TABLEAU) {
        int pos = selected.index % 100;
        if (sol.moveTabtoTab(selected.index / 100, pos, current.index / 100)) {
            win.select({Stack::NONE, 0});
        }
        return;
    }

    if (current.stack == Stack::BUILD && selected.stack == Stack::TABLEAU) {
        Deck tab = sol.getTableaus()[selected.index / 100];
        int tabIdx = selected.index % 100;
        if (tabIdx != tab.size() - 1) {
            // Cannot move to build unless bottom card in stack
            return;
        }
        if (sol.moveTabToBuild(selected.index / 100, current.index)) {
            win.select({Stack::NONE, 0});
        }
        return;
    }

    if (current.stack == Stack::TABLEAU && selected.stack == Stack::BUILD) {
        if (sol.moveBuildToTab(selected.index, current.index / 100)) {
            win.select({Stack::NONE, 0});
        }
        return;
    }

    if (current.stack == Stack::TABLEAU && selected.stack == Stack::DISCARD) {
        if (sol.moveDiscToTab(current.index / 100)) {
            win.select({Stack::NONE, 0});
        }
        return;
    }

    if (current.stack == Stack::BUILD && selected.stack == Stack::DISCARD) {
        if (sol.moveDiscToBuild(current.index)) {
            win.select({Stack::NONE, 0});
        }
    }
}

void Game::focusLastInTab(int tabIdx) {
    Deck tab = sol.getTableaus()[tabIdx];
    int lastIndex = tab.size() - 1;
    if (lastIndex < 0)
        lastIndex = 0;
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
        // Places you in 2nd tab bottom card if discard pile is less than 3
        // else go to 3rd tab bottom card
        if (sol.getDiscard().size() < 3) {
            focusLastInTab(1);
            return;
        }
        focusLastInTab(2);
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
    Position current = win.getFocus();
    switch (current.stack) {
        case Stack::DISCARD:
        // Down from discard takes you to bottom of tab 2 if
        // if discard.size() != 3, else to bottom of tab 3
        if (sol.getDeck().size() < 3) {
            focusLastInTab(1);
            return;
        }
        focusLastInTab(2);
        break;
        case Stack::DECK:
        // Down from deck takes you to bottom of tab 1
        focusLastInTab(0);
        break;
        case Stack::BUILD:
        // Down from build takes you to current.index + 3 tab
        focusLastInTab(current.index + 3);
        break;
        case Stack::TABLEAU:
        // If not in bottom card in tab go down
        // else tab 0 focus deck, tab 1, 2 focus discard
        // 3-6 focus build - 3
        {
            Deck tab = sol.getTableaus()[current.index / 100];
            for (auto i = (current.index % 100) + 1; i < tab.size(); i++) {
                if (tab[i].getFacing() == Facing::FRONT) {
                    win.focus({Stack::TABLEAU, current.index + 1});
                    return;
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
                win.focus({Stack::BUILD, (current.index / 100) - 3});
            }
        }
        break;
    }
}

void Game::right() {
    Position current = win.getFocus();
    switch (current.stack) {
        case Stack::DECK:
        win.focus({Stack::DISCARD, 0});
        break;
        case Stack::DISCARD:
        win.focus({Stack::BUILD, 0});
        break;
        case Stack::BUILD:
        if (current.index < 3) {
            win.focus({Stack::BUILD, current.index + 1});
        }
        else {
            win.focus({Stack::DECK, 0});
        }
        break;
        case Stack::TABLEAU:
        {
            int tabIdx = current.index / 100;
            if (tabIdx == 6) {
                focusLastInTab(0);
                return;
            }
            focusLastInTab(tabIdx + 1);
        }
        break;
    }
}

void Game::left() {
    Position current = win.getFocus();
    switch (current.stack) {
        case Stack::BUILD:
        if (current.index > 0) {
            win.focus({Stack::BUILD, current.index - 1});
            return;
        }
        win.focus({Stack::DISCARD, 0});
        break;
        case Stack::DISCARD:
        win.focus({Stack::DECK, 0});
        break;
        case Stack::DECK:
        win.focus({Stack::BUILD, 3});
        break;
        case Stack::TABLEAU:
        {
            int tabIdx = current.index / 100;
           if (tabIdx == 0) {
               focusLastInTab(6);
               return;
           }
           focusLastInTab(tabIdx - 1);
        }
        break;
    }
}