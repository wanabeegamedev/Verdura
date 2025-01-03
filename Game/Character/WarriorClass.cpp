//
// Created by hous on 1/3/25.
//

#include "WarriorClass.h"

WarriorClass::WarriorClass(Attack *_attack, DefenseStrategy *_defenseStrategy,float _delayBetweenAttack)
{
    attack = _attack;
    delayBetweenAttack = _delayBetweenAttack;
    defenseStrategy = _defenseStrategy;
    delayAccumulator = 0.0f;
}

void WarriorClass::doAttack(float deltatime,
    glm::vec3& position,int facingDirection,SoundManager& soundManager) {
    if (delayBetweenAttack <= delayAccumulator) {
        attack->particleManager.releaseFromObjectPool(position,facingDirection,soundManager);
        delayAccumulator = 0.0f;
    }
    else {
        delayAccumulator += deltatime;
    }
}
