//
// Created by hous on 12/25/24.
//

#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"
#include "../Ability/Attack.h"


class Enemy  :public Character
{
public:
    float currentHp{};
    Attack attack;

    void attackSuccessful() override;
    void attackReceived(float damage) override;
};



#endif //ENEMY_H
