//
// Created by hous on 12/30/24.
//

#include "Hero.h"
constexpr float ATTACK_XP_INCREASE = 20.0f;
constexpr float ATTACK_MANA_COST = 5.0f;

Hero::Hero(OBJMesh* _characterMesh,
        const std::string& _name){
        characterMesh = _characterMesh;
        name = _name;
        stats = new Stats();
        // TODO #define ou constexpr
        stats->setHP(200.0f);
        stats->setMana(400.0f);
}

void Hero::attackSuccessful()
{
        stats->currentExp+=ATTACK_XP_INCREASE;
        //Toutes les attaques donnent le meme xp
        stats->currentMana -= ATTACK_MANA_COST;
}
void Hero::attackReceived(float damage)
{
        //Les defenses stratégies reduisent les dégats
        //TODO RetourALEnvoyeurStrategy.h
        for (auto& heroClass: HeroClasses) {
                damage = heroClass->defenseStrategy->doDefense(damage);
        }
        stats->currentHp -= damage;
}

Hero::~Hero() = default;

void Hero::handleInputs(float deltaTime,SoundManager& soundManager) {

        if (ImGui::IsKeyPressed(ImGuiKey_L,false))
        {
                HeroClasses[0]->doAttack(deltaTime,characterMesh->position,
                   characterMesh->facingDirection,soundManager);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_M,false))
        {
                if (HeroClasses.size()>1)
                        HeroClasses[1]->doAttack(deltaTime,characterMesh->position,
                            characterMesh->facingDirection,soundManager);
        }
        characterMesh->handleInputs(deltaTime); //TODO should use  InputHandler class to include imgui jsut once
}
