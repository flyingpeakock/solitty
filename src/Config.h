#pragma once

const int pullCards = 3; // Amount of cards to pull from the deck
                         // More than 4 cards won't display properly

// Card dimensions in characters
const int cardWidth = 7;
const int cardHeight = 5;

// Characters used for drawing the box around the card
enum Characters {
    TOP_LEFT_CARD = L'╭',
    TOP_RIGHT_CARD = L'╮',
    BOT_LEFT_CARD = L'╰',
    BOT_RIGHT_CARD = L'╯',
    VERTICAL_CARD = L'│',
    HORIZONTAL_CARD = L'─',
    BACK_CHARACTER = L'╳',
};

// Characters used to draw the suit
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
    RIGHT = 'l',
    SELECT = ' ',
};

const int cardOffset = 3; // How much the cards overlap in tableau
const int cardOffseHidden = 1; // Offset for turned over cards in the tableau