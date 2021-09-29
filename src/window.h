#pragma once
#include "solitaire.h"
#include "position.h"

class Window {
    public:
    Window();
    Window(Solitaire *s);
    ~Window();

    void changeSolitaire(Solitaire *s);

    void print();

    void focus(Position pos);
    void select(Position pos);

    Position getFocus() const;
    Position getSelect() const;

    void printMessage(std::string message);

    private:
    Solitaire *sol;

    Position focused;
    Position selected;

    void clear();

    void init();
    int maxY, maxX;
    int leftEdge;

    void printDeck() const;
    void printTableaus() const;
    void printBuild() const;
    void printDiscard() const;
    void printPoints() const;

    void calcLeftEdge();

    static std::vector<std::wstring> splitCardString(std::wstringstream &stream);
};