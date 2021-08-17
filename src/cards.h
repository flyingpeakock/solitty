#pragma once
#include "Config.h"
#include <vector>
#include <iosfwd>
#include <string>

enum Color {
    RED,
    BLACK,
};

enum Facing {
    FRONT,
    BACK,
};

class Card {
    public:
    Card(Suit s, int v);
    void print(std::wostream &stream);
    Color color();
    int rank();
    Suit shape();
    void flip();
    void flipDown();
    void flipUp();

    private:
    Facing facing;
    Suit suit;
    int value;
    std::wstring getValueStr();
    template<typename Enumeration>
    wchar_t as_w_char(Enumeration c);
    void printFront(std::wostream &stream);
    void printBack(std::wostream &stream);
};

typedef std::vector<Card> Deck;