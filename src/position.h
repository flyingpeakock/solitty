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
 * To find index for tableu index / Position::magicNumber is tab
 * index % Position::magicNumber card in tab
 */

struct Position {
    Stack stack;
    int index;
    static const int magicNumber = 32;
    bool operator==(const Position &other) const{
        return stack == other.stack && index == other.index;
    }
};