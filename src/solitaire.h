#pragma once
#include "cards.h"
#include "Stopwatch.h"
#include <array>
#include <map>

class Solitaire {
    public:
    Solitaire();
    void startPlaying();
    void stopPlaying();
    bool isPlaying();
    bool isWon();
    void placeDiscard();
    std::string timePlayed();

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
    int getPoints(bool forcePlaying);

    bool placeWinningBuild();

    private:
    int points;
    int winBonus;
    std::array<Deck, 7> tableaus;
    std::array<Deck, 4> build;
    std::map<Card, bool> beenInBuild;
    Deck deck;
    Deck discard;
    Deck usedDiscard;
    Stopwatch timer;
    bool playing;

    bool won;

    void buildTableaus();
    Card deal();
    static Deck buildDeck();
};