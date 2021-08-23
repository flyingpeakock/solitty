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
 * To find index for tableu index / 32 is tab
 * index % 32 card in tab
 */

struct Position {
    Stack stack;
    int index;
    bool operator==(const Position &other) {
        return stack == other.stack && index == other.index;
    }
};