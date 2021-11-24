#include "game.h"
#include <ncurses.h>
#include <chrono>
#include <thread>
#include <sstream>
#include <map>

Game::Game() : win(&sol){}

Game::Game(Solitaire s) : sol(s), win(&sol){}

bool Game::mainLoop() {
    sol.startPlaying();
    while (!sol.isWon() && sol.isPlaying()) {
        win.print();
        const wchar_t input = getch();
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
            sol.stopPlaying();
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
            case Keybinds::TOP:
            top();
            default:
            if (isCardValue(input)) {
                findAndFocusCard(input);
            }
            break;
        }
    }
    if (sol.isWon()) {
        win.select({Stack::NONE, 0});
        win.focus({Stack::NONE, 0});
        while (sol.placeWinningBuild()) {
            win.print();
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }
    }
    std::stringstream message;
    message << "Points: " << sol.getPoints(true) << ". ";
    message << sol.timePlayed() << '.';
    message << "\nPlay again? y/N";
    win.printMessage(message.str());
    return playAgain();
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
        const int pos = selected.index % Position::magicNumber;
        if (sol.moveTabtoTab(selected.index / Position::magicNumber, pos, current.index / Position::magicNumber)) {
            win.select({Stack::NONE, 0});
        }
        else {
            prev.pop_back();
        }
        return;
    }

    if (current.stack == Stack::BUILD && selected.stack == Stack::TABLEAU) {
        const Deck tab = sol.getTableau(selected.index / Position::magicNumber);
        const int tabIdx = selected.index % Position::magicNumber;
        if (tabIdx != tab.size() - 1) {
            // Cannot move to build unless bottom card in stack
            return;
        }
        if (sol.moveTabToBuild(selected.index / Position::magicNumber, current.index)) {
            win.select({Stack::NONE, 0});
        }
        else {
            prev.pop_back();
        }
        return;
    }

    if (current.stack == Stack::TABLEAU && selected.stack == Stack::BUILD) {
        if (sol.moveBuildToTab(selected.index, current.index / Position::magicNumber)) {
            win.select({Stack::NONE, 0});
        }
        else {
            prev.pop_back();
        }
        return;
    }

    if (current.stack == Stack::TABLEAU && selected.stack == Stack::DISCARD) {
        if (sol.moveDiscToTab(current.index / Position::magicNumber)) {
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
    const int size = sol.getTableau(tabIdx).size();
    const int lastIndex = size > 0 ? size - 1 : 0;
    win.focus({Stack::TABLEAU, (tabIdx * Position::magicNumber) + lastIndex});
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
            const Deck tab = sol.getTableau(current.index / Position::magicNumber);
            if (current.index % Position::magicNumber != 0) {
                for (auto i = (current.index % Position::magicNumber) - 1; i >= 0; i--) {
                    if (tab[i].getFacing() == Facing::FRONT) {
                        win.focus({Stack::TABLEAU, current.index - 1});
                        return;
                    }
                }
            }

            const int tabIdx = current.index / Position::magicNumber;
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
        if (sol.getDiscard().size() < 3) {
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
            const Deck tab = sol.getTableau(current.index / Position::magicNumber);
            for (auto i = (current.index % Position::magicNumber) + 1; i < tab.size(); i++) {
                if (tab[i].getFacing() == Facing::FRONT) {
                    win.focus({Stack::TABLEAU, current.index + 1});
                    return;
                }
            }
            const int tabIdx = current.index / Position::magicNumber;
            switch (tabIdx) {
                case 0:
                win.focus({Stack::DECK, 0});
                break;
                case 1:
                case 2:
                win.focus({Stack::DISCARD, 0});
                break;
                default:
                win.focus({Stack::BUILD, (current.index / Position::magicNumber) - 3});
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
            const int tabIdx = current.index / Position::magicNumber;
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
            const int tabIdx = current.index / Position::magicNumber;
            if (tabIdx == 0) {
                focusLastInTab(6);
                return;
            }
            focusLastInTab(tabIdx - 1);
        }
        break;
    }
}

void Game::top() {
    const Position current = win.getFocus();
    if (current.stack != Stack::TABLEAU) {
        return;
    }

    const int col = current.index / current.magicNumber;
    const Deck tab = sol.getTableau(col);
    for (auto i = 0; i < tab.size(); i++) {
        if (tab[i].getFacing() == Facing::FRONT) {
            const Position top = {Stack::TABLEAU, col*Position::magicNumber + i};
            win.focus(top);
            return;
        }
    }
}

bool Game::playAgain() {
    char ans = getch();
    if (ans == 'y' || ans == 'Y') {
        return true;
    }
    return false;
}

bool Game::isCardValue(const wchar_t val) const {
    if ('0' < val && '9' >= val) {
        return true;
    }
    if ('j' == val || 'J' == val) {
        // Will only work with J unless default down key is changed
        return true;
    }
    if ('q' == val || 'Q' == val) {
        // Will only work with Q unless default quit key is changed
        return true;
    }
    if ('k' == val || 'K' == val) {
        // Will only work with K unless default up key is changed
        return true;
    }
    return false;
}

void Game::findAndFocusCard(const wchar_t input) {
    // Ace is impossible to find,
    // wont be used since it's always at the bottom
    // 10 is found by 1
    const auto focus = win.getFocus();
    if (focus.stack != Stack::TABLEAU) {
        return;
    }
    const int tabIdx = focus.index / focus.magicNumber;
    auto currentTab = sol.getTableau(tabIdx);
    std::map<wchar_t, int> m {
        {'1', 10},
        {'2', 2},
        {'3', 3},
        {'4', 4},
        {'5', 5},
        {'6', 6},
        {'7', 7},
        {'8', 8},
        {'9', 9},
        {'j', 11},
        {'J', 11},
        {'q', 12},
        {'Q', 12},
        {'k', 13},
        {'K', 13}
    };
    for (int i = 0; i < currentTab.size(); i++) {
        if (currentTab[i].rank() == m[input] && currentTab[i].getFacing() == Facing::FRONT) {
            const Position newFocus = {Stack::TABLEAU, (tabIdx*Position::magicNumber) + i};
            win.focus(newFocus);
            return;
        }
    }
}