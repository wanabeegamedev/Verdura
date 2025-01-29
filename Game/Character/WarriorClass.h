//
// Created by hous on 1/3/25.
//

#ifndef WARRIORCLASS_H
#define WARRIORCLASS_H
#include "../Ability/Attack.h"
#include "../Ability/DefenseStrategy.h"

#define WIZARD_CLASS_NAME "Wizard"
#define KNIGHT_CLASS_NAME "Knight"
#define WARRIOR_CLASSES_COUNT 2
class WarriorClass {
public:
    Attack* attack;
    DefenseStrategy* defenseStrategy;
    // Je les mets ici au cas de levelUp sur la classe
    float delayBetweenAttack;
    float delayAccumulator;

    WarriorClass(Attack* _attack,DefenseStrategy* _defenseStrategy,float _delayBetweenAttack);
    void doAttack(float deltatime,glm::vec3& position,int facingDirection,SoundManager&);
    void setName(std::string _name);
    std::string name;
};



#endif //WARRIORCLASS_H
