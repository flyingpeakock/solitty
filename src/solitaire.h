#pragma once
#include "cards.h"
#include <array>

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

    Deck getDeck();
    std::array<Deck, 7> getTableaus();
    std::array<Deck, 4> getBuild();
    Deck getDiscard();
    int getUsed();

    private:
    std::array<Deck, 7> tableaus;
    std::array<Deck, 4> build;
    Deck deck;
    Deck discard;
    Deck usedDiscard;

    void buildTableaus();
    Card deal();
    static Deck buildDeck();
};