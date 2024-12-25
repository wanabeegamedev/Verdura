//
// Created by hous on 12/25/24.
//

#ifndef LEVELING_H
#define LEVELING_H
#include <vector>

#include "Character.h"

struct Level {
    float speedMultiplier{};
    float manaMultiplier{};
    float HPMultiplier{};
    int nbAttackUnlocked{};
    int nbInventoryPlacesUnlocked{};
};
// TODO Create Levels , Other option, read level stats from file (more precise)
std::vector<Level> Levels{};

class Leveling {
public:
    int currentLevel{};
    void onGetExpNotify(Character& character);
    void levelUp(Character& character);
};



#endif //LEVELING_H
