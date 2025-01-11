//
// Created by hous on 12/30/24.
//

#include "Hero.h"
constexpr float ATTACK_XP_INCREASE = 20.0f;

Hero::Hero(OBJMesh* _characterMesh,Stats& _stats,
        const std::string& _name):stats(_stats) {
        characterMesh = _characterMesh;
        name = _name;
        // TODO seed WarriorClasses
        // Here
}

void Hero::attackSuccessful()
{
        stats.currentExp+=ATTACK_XP_INCREASE;
        //Toutes les attaques donnent le meme xp
}
void Hero::attackReceived(float damage)
{
        //Les defenses stratégies
        for (auto& heroClass: HeroClasses) {
                damage = heroClass->defenseStrategy->doDefense(damage);
        }
        stats.currentHp -= damage;
}

Hero::~Hero() = default;
