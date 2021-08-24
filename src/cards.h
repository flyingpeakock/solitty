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
    Card(const Suit s, const int v);
    Card(const Card &other);
    void print(std::wostream &stream) const;
    static void printEmpty(std::wostream &stream);
    Color color() const;
    int rank() const;
    Suit shape() const;
    void flipDown();
    void flipUp();
    Facing getFacing() const;
    static void printBack(std::wostream &stream);

    private:
    Facing facing;
    Suit suit;
    int value;
    std::wstring getValueStr() const;
    template<typename Enumeration>
    static wchar_t as_w_char(const Enumeration c);
    void printFront( std::wostream &stream) const;
    static void printTopRow(std::wostream &stream);
    static void printBotRow(std::wostream &stream);
    static void printEmptyRow(std::wostream &stream, const int lines);
};

typedef std::vector<Card> Deck;