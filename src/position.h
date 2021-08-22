#pragma once

/* Stack is used to describe what stack
 * is being referred to
 */

enum Stack {
    NONE,
    DECK,
    DISCARD,
    BUILD,
    TABLEAU,
};

/* Position struct to determine what card has focus
 * index is only useful for build and tableau.
 * The index for build is 0-4
 * The index for tableau is 0 - 700
 * Where the last tableu starts with 7 and the last two
 * digits are which specific card in the tablue is selected.
 */

struct Position {
    Stack stack;
    int index;
    bool operator==(const Position &other) {
        return stack == other.stack && index == other.index;
    }
};