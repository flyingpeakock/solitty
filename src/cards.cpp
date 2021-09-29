#include "cards.h"
#include <iostream>

Card::Card(const Suit s, const int v): suit(s), value(v), facing(Facing::BACK) {}

Card::Card(const Card &other): facing(other.facing), suit(other.suit), value(other.value) {}

std::wstring Card::getValueStr() const{
    if (value <= 1) {
        return L"A";
    }

    if (value >= 2 && value <= 9) {
        return std::wstring(1, (char) value + '0');
    }

    if (value == 10) {
        return L"10";
    }
    
    // Values are now 10 or above, i.e jack and above
    const std::wstring values[] = {L"J", L"Q", L"K"};
    const int idx = value - 11;
    return values[idx];
}

template<typename Enumeration>
wchar_t Card::as_w_char(const Enumeration c) {
    return static_cast<wchar_t>(c);
}

void Card::print(std::wostream &stream) const{
    switch (facing) {
        case Facing::FRONT:
        return printFront(stream);
        case Facing::BACK:
        return printBack(stream);
    }
}

void Card::printEmpty(std::wostream &stream) {
    // Top row
    printTopRow(stream);

    // Second to fourth row
    printEmptyRow(stream, cardHeight - 2);

    // Bottom row
    printBotRow(stream);

}

void Card::printTopRow(std::wostream &stream) {
    stream << as_w_char(Characters::TOP_LEFT_CARD);
    for (auto i = 0; i < cardWidth - 2; i++) {
        stream << as_w_char(Characters::HORIZONTAL_CARD);
    }
    stream << as_w_char(Characters::TOP_RIGHT_CARD);
    stream << '\n';

}

void Card::printBotRow(std::wostream &stream) {
    stream << as_w_char(Characters::BOT_LEFT_CARD);
    for (auto i = 0; i < cardWidth - 2; i++) {
        stream << as_w_char(Characters::HORIZONTAL_CARD);
    }
    stream << as_w_char(Characters::BOT_RIGHT_CARD);
    stream << '\n';
}

void Card::printEmptyRow(std::wostream &stream, const int lines) {
    for (auto line = 0; line < lines; line++) {
        stream << as_w_char(Characters::VERTICAL_CARD);
        for (auto i = 0; i < cardWidth - 2; i++) {
            stream << L' ';
        }
        stream << as_w_char(Characters::VERTICAL_CARD);
        stream << '\n';
    }
}

void Card::printBack(std::wostream &stream) {
    // Top row
    printTopRow(stream);

    // Second row to fourth row
    for (auto i = 0; i < cardHeight - 2; i++) {
        stream << as_w_char(Characters::VERTICAL_CARD);
        for (auto j = 0; j < cardWidth - 2; j++) {
            stream << as_w_char(Characters::BACK_CHARACTER);
        }
        stream << as_w_char(Characters::VERTICAL_CARD);
        stream << '\n';
    }

    // Bottom row
    printBotRow(stream);
}

void Card::printFront(std::wostream &stream) const{
    printTopRow(stream);

    stream << as_w_char(Characters::VERTICAL_CARD);
    stream << as_w_char(suit);
    for (auto i = 0; i < cardWidth - 3; i++) {
        stream << L" ";
    }
    stream << as_w_char(Characters::VERTICAL_CARD);
    stream << '\n';

    printEmptyRow(stream, (cardHeight / 2) - 2);

    std::wstring val = getValueStr();
    const int length = cardWidth % 2 == 0? val.length() + 1: val.length();
    stream << as_w_char(Characters::VERTICAL_CARD);
    for (auto i = 0; i < (cardWidth - 2) / 2; i++) {
        stream << L' ';
    }
    stream << val;
    for (auto i = 0; i < ((cardWidth - 2) / 2) - length + 1; i++) {
        stream << L' ';
    }
    stream << as_w_char(Characters::VERTICAL_CARD);
    stream << '\n';

    const int lines = (cardHeight / 2) - 1 - ((cardHeight + 1) % 2);
    printEmptyRow(stream, lines);

    // Bottom row
    printBotRow(stream);
}

Color Card::color() const{
    switch (suit) {
        case Suit::HEARTS:
        case Suit::DIAMONDS:
        return Color::RED;
        case Suit::SPADE:
        case Suit::CLUB:
        default:
        return Color::BLACK;
    }
}

void Card::flipDown() {
    facing = Facing::BACK;
}

void Card::flipUp() {
    facing = Facing::FRONT;
}

int Card::rank() const{
    return value;
}

Suit Card::shape() const{
    return suit;
}

Facing Card::getFacing() const{
    return facing;
}

bool Card::operator<(const Card &right) const {
    if (suit == right.suit) {
        return value < right.value;
    }
    return (suit < right.suit);
}