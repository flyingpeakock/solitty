#include "solitaire.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>

Solitaire::Solitaire() {
    deck = buildDeck();
    buildTableaus();
    placeDiscard();
}

Deck Solitaire::buildDeck() {
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
    unsigned seed = std::chrono::system_clock::now()
        .time_since_epoch()
        .count();
    shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
    return deck;
}

Card Solitaire::deal() {
    Card card = deck[0];
    deck.erase(deck.begin());
    return card;
}

void Solitaire::buildTableaus() {
    for (auto i = 0; i < 7; i++) {
        for (auto j = 0; j <= i; j++) {
            tableaus[i].push_back(deal());
        }
    }

    for (auto &tab: tableaus) {
        tab.end()->flip();
    }
}

void Solitaire::placeDiscard() {
    if (deck.size() == 0) {
        deck = discard;
        for (auto &card : deck) {
            card.flipDown();
        }
        discard.clear();
    }
    else if (deck.size() < 3) {
        for (auto &card : deck) {
            discard.emplace_back(deal());
        }
    }
    else {
        for (auto i = 0; i < 3; i++) {
            discard.emplace_back(deal());
        }
    }

    for (auto &card : discard) {
        card.flipUp();
    }
}

void Solitaire::moveTabtoTab(const int fromTab, const int pos, const int toTab) {
    Card from = tableaus[fromTab][pos];
    Card to = tableaus[toTab][tableaus[toTab].size()];

    if (from.color() == to.color()) {
        // Cannot move since colors are the same
        return;
    }

    if (to.rank() - from.rank() != 1) {
        // Connot move since not ascending
        return;
    }

    while (tableaus[fromTab].size() > pos) {
        tableaus[toTab].push_back(tableaus[fromTab][pos]);
        tableaus[fromTab].erase(tableaus[fromTab].begin() + pos);
    }

    // Flipping newly uncovered card
    tableaus[fromTab].end() -> flipUp();
}

void Solitaire::moveTabToBuild(const int fromTab, const int toBuild) {
    Card from = tableaus[fromTab].back();
    if (build[toBuild].size() == 0) {
        // Trying to move non ace to empty build deck
        if (from.rank() != 1)
            return;
        
        build[toBuild].push_back(from);
        tableaus[fromTab].erase(tableaus[fromTab].end());
        tableaus[fromTab].end()->flipUp();
        return;
    }

    Card to = build[toBuild].back();
    if (from.shape() != to.shape()) {
        // Trying to move onto the wrong build
        return;
    }

    if (from.rank() - to.rank() != 1) {
        // Not ascending
        return;
    }

    build[toBuild].push_back(from);
    tableaus[fromTab].erase(tableaus[fromTab].end());
    tableaus[fromTab].end()->flipUp();
}

void Solitaire::moveBuildToTab(const int fromBuild, const int toTab) {
    Card from = build[fromBuild].back();
    Card to = tableaus[toTab].back();

    if (from.color() == to.color()) {
        // Placing same color
        return;
    }

    if (to.rank() - from.rank() != 1) {
        // Not ascending
        return;
    }

    tableaus[toTab].push_back(from);
    build[fromBuild].erase(build[fromBuild].end());
}

void Solitaire::moveDiscToTab(const int toTab) {
    Card from = discard.back();
    if (tableaus[toTab].size() == 0) {
        if (from.rank() == 13) {
            // King, must be placed on empty tab
            tableaus[toTab].push_back(from);
            discard.erase(discard.end());
        }
        return;
    }
    
    Card to = tableaus[toTab].back();
    if (to.color() == from.color()) {
        // Not different colors
        return;
    }

    if (to.rank() - from.rank() != 1) {
        // Not ascending
        return;
    }

    tableaus[toTab].push_back(from);
    discard.erase(discard.end());
}

bool Solitaire::isWon() {
    if (deck.size() == 0 && discard.size() == 0) {
        return true;
    }
    return false;
}