#include "window.h"
#include <locale.h>
#include <ncurses.h>
#include <sstream>

Window::Window(Solitaire *s) : sol(s) {
    init();
}

Window::Window() {
    init();
}

Window::~Window() {
    endwin();
}

void Window::init() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak(); // Get input before enter is pressed
    noecho(); // Don't show key presses
    keypad(stdscr, true); // Allow the use of arrow keys
    curs_set(0); // Hide the cursor

    focused = {Stack::DECK, 0};
    selected = {Stack::NONE, 0};

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

void Window::changeSolitaire(Solitaire *s) {
    sol = s;
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

    const int width = (cardWidth * 7) + (stackSpacing * 6);
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
    clear();
    if (oldY != maxY || oldX != maxX) {
        // Screen has been resized
        calcLeftEdge();
    }

    printDeck();
    printDiscard();
    printTableaus();
    printBuild();
    printPoints();
    refresh();
}

void Window::printDeck() const{
    int row = 0;
    std::wstringstream stream;
    Deck deck = sol->getDeck();
    if (deck.size() == 0) {
        Card::printEmpty(stream);
    }
    else {
        Card::printBack(stream);
    }
    const std::vector<std::wstring> strings = splitCardString(stream);

    if (focused.stack == Stack::DECK) {
        attron(A_BOLD);
    }
    if (selected.stack == Stack::DECK) {
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

void Window::printDiscard() const{
    const Deck discard = sol->getDiscard();
    if (discard.size() == 0 && sol->getUsed() == 0) {
        // Print empty card
        std::wstringstream stream;
        Card::printEmpty(stream);
        const std::vector<std::wstring> strings = splitCardString(stream);
        int row = 0;
        if (selected.stack == Stack::DISCARD) {
            attron(A_BLINK);
        }
        if (focused.stack == Stack::DISCARD) {
            attron(A_BOLD);
        }
        attron(COLOR_PAIR(5));
        for (auto &str : strings) {
            mvaddwstr(row++, leftEdge + cardWidth + stackSpacing, str.c_str());
        }
        attroff(COLOR_PAIR(5));
        attroff(A_BLINK);
        attroff(A_BOLD);
        return;
    }

    if (discard.size() == 0) {
        // Print back of card
        std::wstringstream stream;
        Card::printBack(stream);
        const std::vector<std::wstring> strings = splitCardString(stream);
        int row = 0; 
        if (selected.stack == Stack::DISCARD) {
            attron(A_BLINK);
        }
        if (focused.stack == Stack::DISCARD) {
            attron(A_BOLD);
        }
        attron(COLOR_PAIR(4));
        for (auto &str : strings) {
            mvaddwstr(row++, leftEdge + cardWidth + stackSpacing, str.c_str());
        }
        attroff(COLOR_PAIR(4));
        attroff(A_BOLD);
        attroff(A_BLINK);
        return;
    }

    int col = leftEdge + cardWidth + stackSpacing;
    for (auto i = 0; i < discard.size(); i++) {
        Card card = discard[i];
        std::wstringstream stream;
        card.print(stream);
        const std::vector<std::wstring> strings = splitCardString(stream);
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
        if (i == discard.size() - 1 && selected.stack == Stack::DISCARD) {
            attron(A_BLINK);
        }
        for (auto &str : strings) {
            mvaddwstr(row++, col, str.c_str());
        }
        attroff(COLOR_PAIR(2));
        attroff(COLOR_PAIR(3));
        attroff(A_BOLD);
        attroff(A_BLINK);
        col += (cardWidth / 2) + 1;
    }
}

void Window::printTableaus() const{
    const auto tabs = sol->getTableaus();
    int col = leftEdge;
    for (auto i = 0; i < tabs.size(); i++) {
        const auto tab = tabs[i];
        // Checking if any cards in tab
        int row = cardHeight + 1;
        if (tab.size() == 0) {
            std::wstringstream stream;
            Card::printEmpty(stream);
            const auto str = splitCardString(stream);
            attron(COLOR_PAIR(5));
            if (focused.stack == Stack::TABLEAU && focused.index / Position::magicNumber == i) {
                attron(A_BOLD);
            }
            if (selected.stack == Stack::TABLEAU &&  selected.index / Position::magicNumber == i) {
                attron(A_BLINK);
            }
            for (auto i = 0; i < str.size(); i++) {
                mvaddwstr(row + i, col, str[i].c_str());
            }
            attroff(COLOR_PAIR(5));
            attroff(A_BOLD);
            attroff(A_BLINK);
            col += cardWidth + stackSpacing;
            continue;
        }
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
            if (focused.stack == Stack::TABLEAU && focused.index / Position::magicNumber == i && focused.index % Position::magicNumber == j) {
                attron(A_BOLD);
            }
            if (selected.stack == Stack::TABLEAU && selected.index / Position::magicNumber == i && selected.index % Position::magicNumber <= j) {
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
            if (card.getFacing() == Facing::FRONT) {
                row += cardOffset;
            }
            else {
                row += cardOffseHidden;
            }
        }
        col += cardWidth + stackSpacing;
    }
}

void Window::printBuild() const{
    int row = 0;
    int col = (cardWidth * 3) + (3 * stackSpacing) + leftEdge;
    const auto build = sol->getBuild();
    // for (auto &b : build) {
    for (auto i = 0; i < build.size(); i++) {
        const auto b = build[i];

        if (focused.stack == Stack::BUILD && focused.index == i) {
            attron(A_BOLD);
        }
        if (selected.stack == Stack::BUILD && selected.index == i) {
            attron(A_BLINK);
        }

        if (b.size() == 0) {
            std::wstringstream stream;
            Card::printEmpty(stream);
            const auto str = splitCardString(stream);
            attron(COLOR_PAIR(5));
            for (auto i = 0; i < str.size(); i++) {
                mvaddwstr(row + i, col, str[i].c_str());
            }
            attroff(COLOR_PAIR(5));
            attroff(A_BOLD);
            attroff(A_BLINK);
            col += cardWidth + stackSpacing;
            continue;
        }
        const Card topCard = b.back();
        std::wstringstream stream;
        topCard.print(stream);
        const auto str = splitCardString(stream);
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
        col += cardWidth + stackSpacing;
    }
}

void Window::focus(Position pos) {
    focused = pos;
}

void Window::select(Position pos) {
    selected = pos;
}

Position Window::getFocus() const{
    return focused;
}

Position Window::getSelect() const{
    return selected;
}

void Window::printMessage(std::string message) {
    const int length = message.size();
    move(maxY / 2, (maxX - length) / 2);
    attron(COLOR_PAIR(1));
    attron(A_BOLD);
    addstr(message.c_str());
    attroff(COLOR_PAIR(1));
    attroff(A_BOLD);
    refresh();
}

void Window::printPoints() const {
    std::stringstream sstream;
    sstream << sol->getPoints(true);
    move(maxY - 1, 1);
    attron(COLOR_PAIR(1));
    attron(A_BOLD);
    addstr(sstream.str().c_str());
    attroff(COLOR_PAIR(1));
    attroff(A_BOLD);
}
