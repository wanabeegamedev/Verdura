//
// Created by hous on 12/25/24.
//

#ifndef STATS_H
#define STATS_H
#include <cstdint>

class Stats {
public:
    uint8_t currentLvl{};
    float currentHp{};
    float currentExp{};
    int currentMana{};

    void setHP(float _hp) {
        currentHp = _hp;
    }
    void setMana(float _mana) {
        currentMana = _mana;
    }
};



#endif //STATS_H
