#pragma once
#include "solitaire.h"

class Window {
    public:
    Window();
    void print();

    private:
    Solitaire sol;
    void init();
    int maxY, maxX;

    void printDeck();
    void printTableaus();
    void printBuild();
    void printDiscard();
};