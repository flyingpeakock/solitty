#include "game.h"
#include "Stopwatch.h"
#include <ncurses.h>
#include <chrono>
#include <thread>

Game::Game() : win(&sol){}

void Game::mainLoop() {
    bool playing = true;
    Stopwatch::start();
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
    Stopwatch::stop();
    if (sol.isWon()) {
        while (sol.placeWinningBuild()) {
            win.select({Stack::NONE, 0});
            win.focus({Stack::NONE, 0});
            win.print();
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }
        win.printMessage(Stopwatch::timeTaken());
        getch();
    }
}

void Game::select() {
    const Position current = win.getFocus(); // from
    const Position selected = win.getSelect(); // to

    if (current.stack == Stack::DECK) {
        prev.push_back(sol);
        sol.placeDiscard();
        return;
    }

    if (selected.stack == Stack::NONE) {
        win.select(current);
        return;
    }
    prev.push_back(sol);

    if (current == selected) {
        win.select({Stack::NONE, 0});
        return;
    }

    if (current.stack == Stack::TABLEAU && selected.stack == Stack::TABLEAU) {
        const int pos = selected.index % 32;
        if (sol.moveTabtoTab(selected.index / 32, pos, current.index / 32)) {
            win.select({Stack::NONE, 0});
        }
        else {
            prev.pop_back();
        }
        return;
    }

    if (current.stack == Stack::BUILD && selected.stack == Stack::TABLEAU) {
        const Deck tab = sol.getTableaus()[selected.index / 32];
        const int tabIdx = selected.index % 32;
        if (tabIdx != tab.size() - 1) {
            // Cannot move to build unless bottom card in stack
            return;
        }
        if (sol.moveTabToBuild(selected.index / 32, current.index)) {
            win.select({Stack::NONE, 0});
        }
        else {
            prev.pop_back();
        }
        return;
    }

    if (current.stack == Stack::TABLEAU && selected.stack == Stack::BUILD) {
        if (sol.moveBuildToTab(selected.index, current.index / 32)) {
            win.select({Stack::NONE, 0});
        }
        else {
            prev.pop_back();
        }
        return;
    }

    if (current.stack == Stack::TABLEAU && selected.stack == Stack::DISCARD) {
        if (sol.moveDiscToTab(current.index / 32)) {
            win.select({Stack::NONE, 0});
        }
        else {
            prev.pop_back();
        }
        return;
    }

    if (current.stack == Stack::BUILD && selected.stack == Stack::DISCARD) {
        if (sol.moveDiscToBuild(current.index)) {
            win.select({Stack::NONE, 0});
        }
        else {
            prev.pop_back();
        }
    }
}

void Game::focusLastInTab(const int tabIdx) {
    const Deck tab = sol.getTableaus()[tabIdx];
    const int lastIndex = tab.size() > 0 ? tab.size() - 1 : 0;
    win.focus({Stack::TABLEAU, (tabIdx * 32) + lastIndex});
}

void Game::up() {
    const Position current = win.getFocus();
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
            const Deck tab = sol.getTableaus()[current.index / 32];
            if (current.index % 32 != 0) {
                for (auto i = (current.index % 32) - 1; i >= 0; i--) {
                    if (tab[i].getFacing() == Facing::FRONT) {
                        win.focus({Stack::TABLEAU, current.index - 1});
                        return;
                    }
                }
            }

            const int tabIdx = current.index / 32;
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
    const Position current = win.getFocus();
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
            const Deck tab = sol.getTableaus()[current.index / 32];
            for (auto i = (current.index % 32) + 1; i < tab.size(); i++) {
                if (tab[i].getFacing() == Facing::FRONT) {
                    win.focus({Stack::TABLEAU, current.index + 1});
                    return;
                }
            }
            const int tabIdx = current.index / 32;
            switch (tabIdx) {
                case 0:
                win.focus({Stack::DECK, 0});
                break;
                case 1:
                case 2:
                win.focus({Stack::DISCARD, 0});
                break;
                default:
                win.focus({Stack::BUILD, (current.index / 32) - 3});
            }
        }
        break;
    }
}

void Game::right() {
    const Position current = win.getFocus();
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
            const int tabIdx = current.index / 32;
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
    const Position current = win.getFocus();
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
            const int tabIdx = current.index / 32;
            if (tabIdx == 0) {
                focusLastInTab(6);
                return;
            }
            focusLastInTab(tabIdx - 1);
        }
        break;
    }
}
