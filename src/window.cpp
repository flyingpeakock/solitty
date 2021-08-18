#include "window.h"
#include <locale.h>
#include <ncurses.h>
#include <sstream>

Window::Window() {
    setlocale(LC_ALL, "");
    initscr();
    init();
}

Window::~Window() {
    endwin();
}

void Window::init() {
    cbreak(); // Get input before enter is pressed
    noecho(); // Don't show key presses
    keypad(stdscr, true); // Allow the use of arrow keys

    if (has_colors()) {
        // TODO: Add color pairs that will be defined in config.h
        use_default_colors();
        start_color();
    }
    getmaxyx(stdscr, maxY, maxX);
    calcLeftEdge();
}

void Window::calcLeftEdge() {
    // There are 7 tableaus and 6 gaps between them
    // The gaps will have the widgh of 1 char

    const int width = (cardWidth * 7) + 6;
    leftEdge = (maxX - width) / 2;
}

std::vector<std::wstring>Window::splitCardString(std::wstringstream &stream) {
    std::wstring string;
    
    std::vector<std::wstring> ret;
    while(std::getline(stream, string, L'\n')) {
        ret.push_back(string);
    }
    return ret;
}

void Window::print() {
    int oldY = maxY;
    int oldX = maxX;
    getmaxyx(stdscr, maxY, maxX);
    if (oldY != maxY || oldX != maxX) {
        // Screen has been resized
        clear();
        calcLeftEdge();
    }

    printDeck();
    printDiscard();
    printTableaus();
    printBuild();
    refresh();
}

void Window::printDeck() {
    int row = 0;
    std::wstringstream stream;
    Deck deck = sol.getDeck();
    if (deck.size() == 0) {
        Card::printEmpty(stream);
    }
    else {
        deck.back().print(stream);
    }
    std::vector<std::wstring> strings = splitCardString(stream);

    for (auto &str : strings) {
        mvaddwstr(row++, leftEdge, str.c_str());
    }
}

void Window::printDiscard() {
    Deck discard = sol.getDiscard();
    if (discard.size() == 0) {
        // Print empty card
        std::wstringstream stream;
        Card::printEmpty(stream);
        std::vector<std::wstring> strings = splitCardString(stream);
        int row = 0;
        for (auto &str : strings) {
            mvaddwstr(row++, leftEdge + cardWidth + 1, str.c_str());
        }
        return;
    }

    int col = leftEdge + cardWidth + 1;
    for (auto i = discard.size() - 3; i < discard.size(); i++) {
        Card card = discard[i];
        std::wstringstream stream;
        card.print(stream);
        std::vector<std::wstring> strings = splitCardString(stream);
        int row = 0;
        for (auto &str : strings) {
            mvaddwstr(row++, col, str.c_str());
        }
        col += 4;
    }
}

void Window::printTableaus() {
    auto tabs = sol.getTableaus();
    int col = leftEdge;
    for (auto &tab : tabs) {
        // Checking if any cards in tab
        int row = cardHeight + 1;
        if (tab.size() == 0) {
            std::wstringstream stream;
            Card::printEmpty(stream);
            auto str = splitCardString(stream);
            for (auto i = 0; i < str.size(); i++) {
                mvaddwstr(row + i, col, str[i].c_str());
            }
            col += cardWidth + 1;
            continue;
        }
        for (auto &card : tab) {
            std::wstringstream stream;
            card.print(stream);
            auto str = splitCardString(stream);
            for (auto i = 0; i < str.size(); i++) {
                mvaddwstr(row + i, col, str[i].c_str());
            }
            row += 2;
        }
        col += cardWidth + 1;
    }
}

void Window::printBuild() {
    int row = 0;
    int col = (cardWidth * 3) + 3 + leftEdge;
    auto build = sol.getBuild();
    for (auto &b : build) {
        if (b.size() == 0) {
            std::wstringstream stream;
            Card::printEmpty(stream);
            auto str = splitCardString(stream);
            for (auto i = 0; i < str.size(); i++) {
                mvaddwstr(row + i, col, str[i].c_str());
            }
            col += cardWidth + 1;
            continue;
        }
        Card topCard = b.back();
        std::wstringstream stream;
        topCard.print(stream);
        auto str = splitCardString(stream);
        for (auto i = 0; i < str.size(); i++) {
            mvaddwstr(row + i, col, str[i].c_str());
        }
        col += cardWidth + 1;
    }
}