//
// Created by hous on 12/25/24.
//

#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"
#include "../Ability/Attack.h"
#include <glm/glm.hpp>


class Enemy  :public Character
{
public:
    float currentHp{};
    Attack* attack{};


    void attackSuccessful() override;
    void attackReceived(float damage) override;

    void setAttack(Attack* _attack);

    explicit Enemy(OBJMesh& _characterMesh);
    void alignToHero(const glm::vec3& heroPosition);
    ~Enemy() override;
};



#endif //ENEMY_H
