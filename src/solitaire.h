#pragma once
#include "cards.h"
#include <array>
#include <map>

class Solitaire {
    public:
    Solitaire();
    bool isWon();
    void placeDiscard();

    bool moveTabtoTab(const int fromTab, const int pos, const int toTab);
    bool moveTabToBuild(const int fromTab, const int toBuild);
    bool moveBuildToTab(const int fromBuild, const int toTab);
    bool moveDiscToTab(const int toTab);
    bool moveDiscToBuild(const int toBuild);

    Deck getDeck() const;
    std::array<Deck, 7> getTableaus() const;
    Deck getTableau(const int idx) const;
    std::array<Deck, 4> getBuild() const;
    Deck getDiscard() const;
    int getUsed() const;
    int getPoints();

    bool placeWinningBuild();

    private:
    int points;
    std::array<Deck, 7> tableaus;
    std::array<Deck, 4> build;
    std::map<Card, bool> beenInBuild;
    Deck deck;
    Deck discard;
    Deck usedDiscard;

    bool won;

    void buildTableaus();
    Card deal();
    static Deck buildDeck();
};