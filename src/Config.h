#pragma once

enum Characters {
    TOP_LEFT_CARD = L'╭',
    TOP_RIGHT_CARD = L'╮',
    BOT_LEFT_CARD = L'╰',
    BOT_RIGHT_CARD = L'╯',
    VERTICAL_CARD = L'│',
    HORIZONTAL_CARD = L'─',
    BACK_CHARACTER = L'╳',
};

enum Suit {
    HEARTS = L'♥',
    DIAMONDS = L'♦',
    SPADE = L'♠',
    CLUB = L'♣',
};

// Colors
/* Available colors are:
 * COLOR_BLACK = 0
 * COLOR_RED = 1
 * COLOR_GREEN = 2
 * COLOR_YELLOW = 3
 * COLOR_BLUE = 4
 * COLOR_MAGENTA = 5
 * COLOR_CYAN = 6
 * COLOR_WHITE = 7
 * DEFAULT = -1
 */

enum Colors {
    FOREGROUND_COLOR = -1,
    BACKGROUND_COLOR = 2,
    RED_CARD_COLOR = 1,
    BLACK_CARD_COLOR = 0,
    BACK_CARD_COLOR = 0,
    EMPTY_CARD_COLOR = BACK_CARD_COLOR,
    CARD_BACKGROUND_COLOR = 7,
};

enum Keybinds {
    QUIT = 'q',
    LEFT = 'h',
    DOWN = 'j',
    UP = 'k',
    RIGHT = 'l'
    SELECT = ' ',
}