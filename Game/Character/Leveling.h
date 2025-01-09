//
// Created by hous on 12/25/24.
//

#ifndef LEVELING_H
#define LEVELING_H
#include <vector>

#include "Character.h"

struct Level {
    int xpNeeded;
    float speedMultiplier{};
    float scoreReward{};

    Level(int _xpNeeded,float _speedMultiplier, float _scoreReward):
    xpNeeded(_xpNeeded),
    speedMultiplier(_speedMultiplier),scoreReward(_scoreReward) {}
};
// TODO Create Levels , Other option, read level stats from file (more precise)
static Level level1(20,1.1,500);
static Level level2(80,1.2,1500);
static Level level3(200,1.3,2500);
static Level level4(300,1.4,5000);

static std::vector<Level> Levels{level1,level2,level3,level4};


class Leveling {
public:
    bool checkXP(int XP);
};



#endif //LEVELING_H
