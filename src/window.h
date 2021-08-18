#pragma once
#include "solitaire.h"

class Window {
    public:
    Window();
    ~Window();
    void print();

    private:
    Solitaire sol;

    void clear();

    static const int cardWidth = 7;
    static const int cardHeight = 5;

    void init();
    int maxY, maxX;
    int leftEdge;

    void printDeck();
    void printTableaus();
    void printBuild();
    void printDiscard();

    void calcLeftEdge();

    std::vector<std::wstring> splitCardString(std::wstringstream &stream);
};