//
// Created by hous on 1/3/25.
//

#ifndef WARRIORCLASS_H
#define WARRIORCLASS_H
#include "../Ability/Attack.h"
#include "../Ability/DefenseStrategy.h"


class WarriorClass {
public:
    Attack* attack;
    DefenseStrategy* defenseStrategy;
    // Je les mets ici au cas de levelUp
    float delayBetweenAttack;
    float delayAccumulator;
    WarriorClass(Attack* _attack,DefenseStrategy* _defenseStrategy,float _delayBetweenAttack);
    void doAttack(float deltatime,glm::vec3& position,int facingDirection,SoundManager&);
};



#endif //WARRIORCLASS_H
