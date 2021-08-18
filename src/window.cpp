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
    refresh();
}

void Window::printDeck() {
    int row = 0;
    std::wstringstream stream;
    Deck deck = sol.getDeck();
    deck[0].print(stream);
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