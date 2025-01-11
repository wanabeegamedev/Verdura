//
// Created by hous on 12/30/24.
//

#include "Hero.h"
constexpr float ATTACK_XP_INCREASE = 20.0f;

Hero::Hero(OBJMesh* _characterMesh,
        const std::string& _name){
        characterMesh = _characterMesh;
        name = _name;
        stats = new Stats();
        stats->setHP(200.0f);
}

void Hero::attackSuccessful()
{
        stats->currentExp+=ATTACK_XP_INCREASE;
        //Toutes les attaques donnent le meme xp
}
void Hero::attackReceived(float damage)
{
        //Les defenses stratégies
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
                if (HeroClasses[1]!=nullptr)
                        HeroClasses[1]->doAttack(deltaTime,characterMesh->position,
                            characterMesh->facingDirection,soundManager);
        }
        characterMesh->handleInputs(deltaTime); //TODO should use a specific InputHandler class to include imgui jsut once
}
