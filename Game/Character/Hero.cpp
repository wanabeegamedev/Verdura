//
// Created by hous on 12/30/24.
//

#include "Hero.h"
constexpr float ATTACK_XP_INCREASE = 10.0f;

Hero::Hero(OBJMesh* _characterMesh,Stats& _stats, Inventory& _inventory,
        const std::string& _name):stats(_stats) {
        characterMesh = _characterMesh;
        inventory = _inventory;
        name = _name;
        //TODO seed WarriorClasses
}

void Hero::attackSuccessful()
{
        stats.currentExp+=ATTACK_XP_INCREASE;
        //Toutes les attaques donnent le meme xp
}
void Hero::attackReceived(float damage)
{
        stats.currentHp -= damage;
}

Hero::~Hero() = default;
