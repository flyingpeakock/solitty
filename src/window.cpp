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
    curs_set(0); // Hide the cursor

    if (has_colors()) {
        // TODO: Add color pairs that will be defined in config.h
        use_default_colors();
        start_color();
        init_pair(1, FOREGROUND_COLOR, BACKGROUND_COLOR);
        init_pair(2, RED_CARD_COLOR, CARD_BACKGROUND_COLOR);
        init_pair(3, BLACK_CARD_COLOR, CARD_BACKGROUND_COLOR);
        init_pair(4, BACK_CARD_COLOR, CARD_BACKGROUND_COLOR);
        init_pair(5, EMPTY_CARD_COLOR, CARD_BACKGROUND_COLOR);
    }
    getmaxyx(stdscr, maxY, maxX);
    calcLeftEdge();
    clear();
}

void Window::clear() {
    move(0,0);
    attron(COLOR_PAIR(1));
    for (auto i = 0; i < maxY*maxX; i++) {
        addch(' ');
    }
    attroff(COLOR_PAIR(1));
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

    if (focused.stack == Stack::DECK) {
        attron(A_BOLD);
    }
    else if (selected.stack == Stack::DECK) {
        attron(A_BLINK);
    }

    attron(COLOR_PAIR(4));
    for (auto &str : strings) {
        mvaddwstr(row++, leftEdge, str.c_str());
    }
    attroff(COLOR_PAIR(4));
    attroff(A_BOLD);
    attroff(A_BLINK);
}

void Window::printDiscard() {
    Deck discard = sol.getDiscard();
    if (discard.size() == 0) {
        // Print empty card
        std::wstringstream stream;
        Card::printEmpty(stream);
        std::vector<std::wstring> strings = splitCardString(stream);
        int row = 0;
        if (selected.stack == Stack::DISCARD) {
            attron(A_BLINK);
        }
        else if (focused.stack == Stack::DISCARD) {
            attron(A_BOLD);
        }
        attron(COLOR_PAIR(5));
        for (auto &str : strings) {
            mvaddwstr(row++, leftEdge + cardWidth + 1, str.c_str());
        }
        attroff(COLOR_PAIR(5));
        attroff(A_BLINK);
        attroff(A_BOLD);
        return;
    }

    int col = leftEdge + cardWidth + 1;
    for (auto i = discard.size() - 3; i < discard.size(); i++) {
        Card card = discard[i];
        std::wstringstream stream;
        card.print(stream);
        std::vector<std::wstring> strings = splitCardString(stream);
        int row = 0;
        if (card.color() == Color::BLACK) {
            attron(COLOR_PAIR(3));
        }
        else if (card.color() == Color::RED) {
            attron(COLOR_PAIR(2));
        }

        if (i == discard.size() - 1 && focused.stack == Stack::DISCARD) {
            attron(A_BOLD);
        }
        else if (i == discard.size() - 1 && selected.stack == Stack::DISCARD) {
            attron(A_BLINK);
        }
        for (auto &str : strings) {
            mvaddwstr(row++, col, str.c_str());
        }
        attroff(COLOR_PAIR(2));
        attroff(COLOR_PAIR(3));
        attroff(A_BOLD);
        attroff(A_BLINK);
        col += 4;
    }
}

void Window::printTableaus() {
    auto tabs = sol.getTableaus();
    int col = leftEdge;
    // for (auto &tab : tabs) {
    for (auto i = 0; i < tabs.size(); i++) {
        auto tab = tabs[i];
        // Checking if any cards in tab
        int row = cardHeight + 1;
        if (tab.size() == 0) {
            std::wstringstream stream;
            Card::printEmpty(stream);
            auto str = splitCardString(stream);
            attron(COLOR_PAIR(5));
            if (focused.stack == Stack::TABLEAU && focused.index / 100 == i) {
                attron(A_BOLD);
            }
            else if (selected.stack == Stack::TABLEAU &&  selected.index / 100 == i) {
                attron(A_BLINK);
            }
            for (auto i = 0; i < str.size(); i++) {
                mvaddwstr(row + i, col, str[i].c_str());
            }
            attroff(COLOR_PAIR(5));
            attroff(A_BOLD);
            attroff(A_BLINK);
            col += cardWidth + 1;
            continue;
        }
        // for (auto &card : tab) {
        for (auto j = 0; j < tab.size(); j++) {
            auto card = tab[j];
            std::wstringstream stream;
            card.print(stream);
            auto str = splitCardString(stream);
            if (card.getFacing() == Facing::BACK) {
                attron(COLOR_PAIR(4));
            }
            else if (card.color() == Color::BLACK) {
                attron(COLOR_PAIR(3));
            }
            else {
                attron(COLOR_PAIR(2));
            }
            if (focused.stack == Stack::TABLEAU && focused.index / 100 == i && focused.index % 100 == j) {
                attron(A_BOLD);
            }
            else if (selected.stack == Stack::TABLEAU && selected.index / 100 == i && selected.index % 100 <= j) {
                attron(A_BLINK);
            }
            for (auto i = 0; i < str.size(); i++) {
                mvaddwstr(row + i, col, str[i].c_str());
            }
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(3));
            attroff(COLOR_PAIR(4));
            attroff(A_BOLD);
            attroff(A_BLINK);
            row += 2;
        }
        col += cardWidth + 1;
    }
}

void Window::printBuild() {
    int row = 0;
    int col = (cardWidth * 3) + 3 + leftEdge;
    auto build = sol.getBuild();
    // for (auto &b : build) {
    for (auto i = 0; i < build.size(); i++) {
        auto b = build[i];

        if (focused.stack == Stack::BUILD && focused.index == i) {
            attron(A_BOLD);
        }
        else if (selected.stack == Stack::BUILD && selected.index == i) {
            attron(A_BLINK);
        }

        if (b.size() == 0) {
            std::wstringstream stream;
            Card::printEmpty(stream);
            auto str = splitCardString(stream);
            attron(COLOR_PAIR(5));
            for (auto i = 0; i < str.size(); i++) {
                mvaddwstr(row + i, col, str[i].c_str());
            }
            attroff(COLOR_PAIR(5));
            attroff(A_BOLD);
            attroff(A_BLINK);
            col += cardWidth + 1;
            continue;
        }
        Card topCard = b.back();
        std::wstringstream stream;
        topCard.print(stream);
        auto str = splitCardString(stream);
        if (topCard.color() == Color::BLACK) {
            attron(COLOR_PAIR(3));
        }
        else {
            attron(COLOR_PAIR(2));
        }
        for (auto i = 0; i < str.size(); i++) {
            mvaddwstr(row + i, col, str[i].c_str());
        }
        attroff(COLOR_PAIR(2));
        attroff(COLOR_PAIR(3));
        attroff(A_BOLD);
        attroff(A_BLINK);
        col += cardWidth + 1;
    }
}

void Window::focus(Position pos) {
    focused = pos;
}

void Window::select(Position pos) {
    selected = pos;
}