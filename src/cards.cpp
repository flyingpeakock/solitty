#include "cards.h"
#include <iostream>

Card::Card(Suit s, int v): suit(s), value(v), facing(Facing::BACK) {};

Card::Card(const Card &other) {
    facing = other.facing;
    suit = other.suit;
    value = other.value;
}

std::wstring Card::getValueStr() {
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
    std::wstring values[] = {L"J", L"Q", L"K"};
    int idx = value - 11;
    return values[idx];
}

template<typename Enumeration>
wchar_t Card::as_w_char(Enumeration c) {
    return static_cast<wchar_t>(c);
}

void Card::print(std::wostream &stream) {
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
    for (auto i = 0; i < cardHeight - 2; i++) {
        stream << as_w_char(Characters::VERTICAL_CARD);
        for (auto j = 0; j < cardWidth - 2; j++) {
            stream << L' ';
        }
        stream << as_w_char(Characters::VERTICAL_CARD);
        stream << '\n';
    }

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
    stream << std::endl;
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

void Card::printFront(std::wostream &stream) {
    printTopRow(stream);

    stream << as_w_char(Characters::VERTICAL_CARD);
    stream << as_w_char(suit);
    for (auto i = 0; i < cardWidth - 3; i++) {
        stream << L" ";
    }
    stream << as_w_char(Characters::VERTICAL_CARD);
    stream << '\n';

    for (auto i = 0; i < (cardHeight / 2) - 2; i++) {
        stream << as_w_char(Characters::VERTICAL_CARD);
        for (auto j = 0; j < cardWidth - 2; j++) {
            stream << L' ';
        }
        stream << as_w_char(Characters::VERTICAL_CARD);
        stream << '\n';
    }

    std::wstring val = getValueStr();
    int length = val.length();
    if (cardWidth % 2 == 0) {
        length++;
    }
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

    for (auto i = 0; i < (cardHeight / 2) - 1 - ((cardHeight + 1) % 2); i++) {
        stream << as_w_char(Characters::VERTICAL_CARD);
        for (auto j = 0; j < cardWidth - 2; j++) {
            stream << L' ';
        }
        stream << as_w_char(Characters::VERTICAL_CARD);
        stream << '\n';
    }

    // Bottom row
    printBotRow(stream);
}

Color Card::color() {
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

void Card::flip() {
    switch (facing) {
        case Facing::FRONT:
        facing = Facing::BACK;
        break;
        case Facing::BACK:
        facing = Facing::FRONT;
        break;
    }
}

void Card::flipDown() {
    facing = Facing::BACK;
}

void Card::flipUp() {
    facing = Facing::FRONT;
}

int Card::rank() {
    return value;
}

Suit Card::shape() {
    return suit;
}

Facing Card::getFacing() {
    return facing;
}