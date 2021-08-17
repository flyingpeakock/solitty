#include "cards.h"
#include <array>

class Solitaire {
    public:
    Solitaire();
    bool isWon();
    void placeDiscard();

    void moveTabtoTab(const int fromTab, const int pos, const int toTab);
    void moveTabToBuild(const int fromTab, const int toBuild);
    void moveBuildToTab(const int fromBuild, const int toTab);
    void moveDiscToTab(const int toTab);

    private:
    std::array<Deck, 7> tableaus;
    std::array<Deck, 4> build;
    Deck deck;
    Deck discard;

    void buildTableaus();
    Card deal();
    static Deck buildDeck();
};