#include "window.h"
#include <locale.h>
#include <ncurses.h>

Window::Window() {
    setlocale(LC_ALL, "");
    initscr();
    init();
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
}

void Window::print() {
    int oldY = maxY;
    int oldX = maxX;
    getmaxyx(stdscr, maxY, maxX);
    if (oldY != maxY || oldX != maxX) {
        // Screen has been resized
        clear();
    }

    printDeck();

    refresh();
}

void Window::printDeck() {
    
}