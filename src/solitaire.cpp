#include "solitaire.h"
#include <algorithm>
#include <random>
#include <chrono>

Solitaire::Solitaire() {
    deck = buildDeck();
    buildTableaus();
    for (auto &card : deck) {
        card.flipUp(); // Flipping entire deck up since window
                       // explicitly prints back anyway
    }
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
    Card card = deck.back();
    deck.pop_back();
    return card;
}

void Solitaire::buildTableaus() {
    for (auto i = 0; i < 7; i++) {
        for (auto j = 0; j <= i; j++) {
            tableaus[i].push_back(deal());
        }
    }

    for (auto &tab: tableaus) {
        tab.back().flipUp();
    }
}

void Solitaire::placeDiscard() {
    usedDiscard.insert(usedDiscard.begin(), discard.begin(), discard.end());
    discard.clear();
    if (deck.size() == 0) {
        deck = usedDiscard;
        usedDiscard.clear();
    }
    else if (deck.size() <= pullCards) {
        for (auto &card : deck) {
            // discard.push_back(deal());
            discard.insert(discard.begin(), deal());
        }
    }
    else {
        for (auto i = 0; i < pullCards; i++) {
            // discard.push_back(deal());
            discard.insert(discard.begin(), deal());
        }
    }
}

bool Solitaire::moveTabtoTab(const int fromTab, const int pos, const int toTab) {
    Card from = tableaus[fromTab][pos];

    if (tableaus[toTab].size() == 0) {
        if (from.rank() != 13)
            return false;
        while (tableaus[fromTab].size() > pos) {
            tableaus[toTab].push_back(tableaus[fromTab][pos]);
            tableaus[fromTab].erase(tableaus[fromTab].begin() + pos);
        }
        tableaus[fromTab].back().flipUp();
        return true;
    }
    Card to = tableaus[toTab][tableaus[toTab].size() - 1];

    if (from.color() == to.color()) {
        // Cannot move since colors are the same
        return false;
    }

    if (to.rank() - from.rank() != 1) {
        // Connot move since not ascending
        return false;
    }

    while (tableaus[fromTab].size() > pos) {
        tableaus[toTab].push_back(tableaus[fromTab][pos]);
        tableaus[fromTab].erase(tableaus[fromTab].begin() + pos);
    }

    // Flipping newly uncovered card
    tableaus[fromTab].back().flipUp();
    return true;
}

bool Solitaire::moveDiscToBuild(const int toBuild) {
    if (discard.size() == 0) {
        return false;
    }
    Card from = discard.back();
    if (build[toBuild].size() == 0) {
        // Trying to move non ace to empty build deck
        if (from.rank() != 1) {
            return false;
        }

        build[toBuild].push_back(from);
        discard.pop_back();
        return true;
    }

    Card to = build[toBuild].back();
    if (from.shape() != to.shape()) {
        // Trying to move onto the wrong build
        return false;
    }

    if (from.rank() - to.rank() != 1) {
        // Not ascending
        return false;
    }

    build[toBuild].push_back(from);
    discard.pop_back();
    return true;
}

bool Solitaire::moveTabToBuild(const int fromTab, const int toBuild) {
    Card from = tableaus[fromTab].back();
    if (build[toBuild].size() == 0) {
        // Trying to move non ace to empty build deck
        if (from.rank() != 1)
            return false;
        
        build[toBuild].push_back(from);
        tableaus[fromTab].pop_back();
        tableaus[fromTab].back().flipUp();
        return true;
    }

    Card to = build[toBuild].back();
    if (from.shape() != to.shape()) {
        // Trying to move onto the wrong build
        return false;
    }

    if (from.rank() - to.rank() != 1) {
        // Not ascending
        return false;
    }

    build[toBuild].push_back(from);
    tableaus[fromTab].pop_back();
    tableaus[fromTab].back().flipUp();
    return true;
}

bool Solitaire::moveBuildToTab(const int fromBuild, const int toTab) {
    Card from = build[fromBuild].back();
    Card to = tableaus[toTab].back();

    if (from.color() == to.color()) {
        // Placing same color
        return false;
    }

    if (to.rank() - from.rank() != 1) {
        // Not ascending
        return false;
    }

    tableaus[toTab].push_back(from);
    build[fromBuild].pop_back();
    return true;
}

bool Solitaire::moveDiscToTab(const int toTab) {
    if (discard.size() == 0) {
        return false;
    }
    Card from = discard.back();
    if (tableaus[toTab].size() == 0) {
        if (from.rank() == 13) {
            // King, must be placed on empty tab
            tableaus[toTab].push_back(from);
            discard.pop_back();
            return true;
        }
        return false;
    }
    
    Card to = tableaus[toTab].back();
    if (to.color() == from.color()) {
        // Not different colors
        return false;
    }

    if (to.rank() - from.rank() != 1) {
        // Not ascending
        return false;
    }

    tableaus[toTab].push_back(from);
    discard.pop_back();
    return true;
}

bool Solitaire::isWon() {
    if (deck.size() == 0 && discard.size() == 0 && usedDiscard.size() == 0) {
        for (auto &tab : tableaus) {
            for (auto &card : tab) {
                if (card.getFacing() == Facing::BACK) {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

Deck Solitaire::getDeck() {
    return deck;
}

std::array<Deck, 7> Solitaire::getTableaus() {
    return tableaus;
}

std::array<Deck, 4> Solitaire::getBuild() {
    return build;
}

Deck Solitaire::getDiscard() {
    return discard;
}

int Solitaire::getUsed() {
    return usedDiscard.size();
}