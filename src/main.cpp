#include <iostream>
#include <locale.h>
#include "solitaire.h"

Deck build_deck() {
    Deck deck;
    for (auto suit = 0; suit < 4; suit++) {
        for (auto i = 1; i <= 13; i++) {
            switch (suit) {
                case 0:
                deck.emplace_back(Card(Suit::HEARTS, i));
                break;
                case 1:
                deck.emplace_back(Card(Suit::DIAMONDS, i));
                break;
                case 2:
                deck.emplace_back(Card(Suit::SPADE, i));
                break;
                case 3:
                deck.emplace_back(Card(Suit::CLUB, i));
                break;
            }
        }
    }
    return deck;
}

int main(int arc, char * argv[]) {
    std::vector<Deck> test;
    setlocale(LC_ALL, "");;
    /*
    Deck deck = build_deck();
    for (auto i = 0; i < deck.size(); i+=2) {
        deck[i].flip();
    }
    for (auto &card : deck) {
        switch (card.color()) {
            case Color::RED:
            // std::wcout << "\033[31;47m";
            break;
            case Color::BLACK:
            // std::wcout << "\033[30;47m";
            break;
        }
        card.print(std::wcout);
    }
    */
   Solitaire sol;
}