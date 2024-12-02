#ifndef GIMIK_H
#define GIMIK_H

#include "imgui.h"
#include "RBTree.h"
#include <random>

class Gimik
{
private:
    RBTree &rbtree;

    // Calculator state
    std::vector<float> numbers;
    std::string expression;
    float result = 0.0f;
    char operation = '+';

    // Dice state
    int diceResult = 1;
    bool isRolling = false;
    float rollDuration = 0.0f;
    std::mt19937 rng;

    // Random book state
    std::vector<Node *> randomBookResult;

    float calculate();
    int rollDice();

public:
    Gimik(RBTree &tree);
    void render();
    void renderCalculator();
    void renderDiceRoller();
    void renderRandomBook();
};

#endif