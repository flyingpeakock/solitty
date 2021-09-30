#include "solitaire.h"
#include <algorithm>
#include <random>
#include <chrono>

Solitaire::Solitaire() {
    won = false;
    playing = false;
    deck = buildDeck();
    points = 0;
    winBonus = 0;
    buildTableaus();
    for (auto &card : deck) {
        card.flipUp(); // Flipping entire deck up since window
                       // explicitly prints back anyway
    }
}

bool Solitaire::isPlaying() {
    return playing;
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
    const unsigned seed = std::chrono::system_clock::now()
        .time_since_epoch()
        .count();
    shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
    return deck;
}

Card Solitaire::deal() {
    const Card card = deck.back();
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
        for (const auto &card : deck) {
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
    const Card from = tableaus[fromTab][pos];

    if (tableaus[toTab].size() == 0) {
        if (from.rank() != 13)
            return false;
        while (tableaus[fromTab].size() > pos) {
            tableaus[toTab].push_back(tableaus[fromTab][pos]);
            tableaus[fromTab].erase(tableaus[fromTab].begin() + pos);
        }
        if (tableaus[fromTab].back().getFacing() == Facing::BACK) {
            points += 5;
        }
        tableaus[fromTab].back().flipUp();
        return true;
    }
    const Card to = tableaus[toTab][tableaus[toTab].size() - 1];

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
    if (tableaus[fromTab].back().getFacing() == Facing::BACK) {
        points += 5;
    }
    tableaus[fromTab].back().flipUp();
    return true;
}

bool Solitaire::moveDiscToBuild(const int toBuild) {
    if (discard.size() == 0) {
        return false;
    }
    const Card from = discard.back();
    if (build[toBuild].size() == 0) {
        // Trying to move non ace to empty build deck
        if (from.rank() != 1) {
            return false;
        }

        build[toBuild].push_back(from);
        discard.pop_back();
        beenInBuild[from] = true;
        points += 10;
        return true;
    }

    const Card to = build[toBuild].back();
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
    beenInBuild[from] = true;
    points += 10;
    return true;
}

bool Solitaire::moveTabToBuild(const int fromTab, const int toBuild) {
    const Card from = tableaus[fromTab].back();
    if (build[toBuild].size() == 0) {
        // Trying to move non ace to empty build deck
        if (from.rank() != 1)
            return false;
        
        build[toBuild].push_back(from);
        tableaus[fromTab].pop_back();
        if (tableaus[fromTab].back().getFacing() == Facing::BACK) {
            points += 5;
        }
        tableaus[fromTab].back().flipUp();
        if (!beenInBuild[from]) {
            points += 10;
        }
        beenInBuild[from] = true;
        return true;
    }

    const Card to = build[toBuild].back();
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
    if (tableaus[fromTab].back().getFacing() == Facing::BACK) {
        points += 5;
    }
    tableaus[fromTab].back().flipUp();
    if (!beenInBuild[from]) {
        points += 10;
    }
    beenInBuild[from] = true;
    return true;
}

bool Solitaire::moveBuildToTab(const int fromBuild, const int toTab) {
    if (build[fromBuild].size() == 0) {
        return false;
    }

    const Card from = build[fromBuild].back();
    if (tableaus[toTab].size() == 0) {
        if (from.rank() != 13) {
            // Trying to move non king to tab
            return false;
        }
        tableaus[toTab].push_back(from);
        build[fromBuild].pop_back();
        beenInBuild[from] = true;
        return true;
    }

    const Card to = tableaus[toTab].back();

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
    beenInBuild[from] = true;
    return true;
}

bool Solitaire::moveDiscToTab(const int toTab) {
    if (discard.size() == 0) {
        return false;
    }
    const Card from = discard.back();
    if (tableaus[toTab].size() == 0) {
        if (from.rank() == 13) {
            // King, must be placed on empty tab
            tableaus[toTab].push_back(from);
            discard.pop_back();
            points += 5;
            return true;
        }
        return false;
    }
    
    const Card to = tableaus[toTab].back();
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
    points += 5;
    return true;
}

bool Solitaire::isWon() {
    if (won) { // Putting check here to avoid unnecessary checks
        return true;
    }

    if (deck.size() == 0 && discard.size() == 0 && usedDiscard.size() == 0) {
        for (const auto &tab : tableaus) {
            for (const auto &card : tab) {
                if (card.getFacing() == Facing::BACK) {
                    return false;
                }
            }
        }
        timer.stop();
        if (winBonus == 0) {
            winBonus = (getPoints() - timer.totalSeconds()) * 10;
        }
        playing = false;
        won = true;
        return true;
    }
    return false;
}

Deck Solitaire::getDeck() const{
    return deck;
}

std::array<Deck, 7> Solitaire::getTableaus() const{
    return tableaus;
}

Deck Solitaire::getTableau(const int idx) const{
    return tableaus[idx];
}

std::array<Deck, 4> Solitaire::getBuild() const{
    return build;
}

Deck Solitaire::getDiscard() const{
    return discard;
}

int Solitaire::getUsed() const{
    return usedDiscard.size();
}

bool Solitaire::placeWinningBuild() {
    if (!isWon()) {
        // This function shouldnt be called unless game is won
        return false;
    }

    int lowestVal = 14;
    int tabIdx = -1;
    //for (auto &tab : tableaus) {
    for (auto i = 0; i < tableaus.size(); i++) {
        auto tab = tableaus[i];
        if (tab.size() > 0 && tab.back().rank() < lowestVal) {
            lowestVal = tab.back().rank();
            tabIdx = i;
        }
    }

    if (tabIdx == -1) {
        return false;
    }

    const Suit s = tableaus[tabIdx].back().shape();
    for (auto i = 0; i < build.size(); i++) {
        if (build[i].size() == 0) {
            if (lowestVal == 1) {
                moveTabToBuild(tabIdx, i);
                return true;
            }
            continue;
        }
        if (s == build[i].back().shape()) {
            moveTabToBuild(tabIdx, i);
            return true;
        }
    }
    return false;
}

int Solitaire::getPoints() {
    return getPoints(false);
}

int Solitaire::getPoints(bool forcePlaying) {
    if (!playing && !forcePlaying) {
        return points;
    }

    int penalty = (timer.totalSeconds() / 15) * 2;
    int adjustedPoints = points - penalty;
    adjustedPoints += winBonus;
    return adjustedPoints > 0 ? adjustedPoints : 0;
}

void Solitaire::startPlaying() {
    playing = true;
    timer.start();
}

void Solitaire::stopPlaying() {
    playing = false;
    timer.stop();
}