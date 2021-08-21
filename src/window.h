#pragma once
#include "solitaire.h"
#include "position.h"

class Window {
    public:
    Window();
    Window(Solitaire *s);
    ~Window();
    void print();

    void focus(Position pos);
    void select(Position pos);

    Position getFocus();
    Position getSelect();

    void printMessage(std::string message);

    private:
    Solitaire *sol;

    Position focused;
    Position selected;

    void clear();

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