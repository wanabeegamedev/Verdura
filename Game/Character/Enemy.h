//
// Created by hous on 12/25/24.
//

#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"
#include "../Ability/Attack.h"
#include <glm/glm.hpp>

#include "WarriorClass.h"
#include "../../Engine/Mesh/OBJMeshFlyWeight.h"


class Enemy  :public Character
{
public:
    float currentHp{};
    WarriorClass* warriorClass{};
    float delayBetweenAttack{};
    OBJMeshFlyWeight* flyweightMesh;

    void attackSuccessful() override;
    void attackReceived(float damage) override;

    void setClass(WarriorClass* _class);

     Enemy(OBJMesh* _characterMesh,OBJMeshFlyWeight* _flyweightMesh);
    void alignToHero(const glm::vec3& heroPosition);
    void doAttack(float deltatime,SoundManager&);
    bool isCloseEnough(const glm::vec3&) const;
    ~Enemy() override;

};



#endif //ENEMY_H
