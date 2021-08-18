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
    Card(const Card &other);
    void print(std::wostream &stream);
    static void printEmpty(std::wostream &stream);
    Color color();
    int rank();
    Suit shape();
    void flip();
    void flipDown();
    void flipUp();
    Facing getFacing();

    private:
    Facing facing;
    Suit suit;
    int value;
    std::wstring getValueStr();
    template<typename Enumeration>
    static wchar_t as_w_char(Enumeration c);
    void printFront(std::wostream &stream);
    void printBack(std::wostream &stream);
    static void printTopRow(std::wostream &stream);
    static void printBotRow(std::wostream &stream);
};

typedef std::vector<Card> Deck;