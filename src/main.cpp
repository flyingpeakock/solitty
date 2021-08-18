#include <iostream>
#include <locale.h>
#include "window.h"
#include <ncurses.h>

int main(int arc, char * argv[]) {
    Window win;
    win.print();
    getch();
}