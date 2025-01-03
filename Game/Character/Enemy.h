//
// Created by hous on 12/25/24.
//

#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"
#include "../Ability/Attack.h"
#include <glm/glm.hpp>

#include "WarriorClass.h"


class Enemy  :public Character
{
public:
    float currentHp{};
    WarriorClass* warriorClass{};
    float delayBetweenAttack{};

    void attackSuccessful() override;
    void attackReceived(float damage) override;

    void setClass(WarriorClass* _class);

    explicit Enemy(OBJMesh* _characterMesh);
    void alignToHero(const glm::vec3& heroPosition);
    void doAttack(float deltatime,SoundManager&);
    ~Enemy() override;

};



#endif //ENEMY_H
