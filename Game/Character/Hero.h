//
// Created by hous on 12/30/24.
//

#ifndef HERO_H
#define HERO_H
#include "Character.h"
#include "DamageManager.h"
#include "WarriorClass.h"
#include "imgui.h"
#include "Leveling.h"
#include "../Engine/Sound/SoundManager.h"

class Hero:public Character{
    public:

    std::vector<WarriorClass*> HeroClasses;
    Hero(OBJMesh* _characterMesh,
        const std::string& _name);

    void attackSuccessful() override;
    void attackReceived(float damage) override;

    ~Hero() override;
    std::string name;
    Stats* stats;
    void handleInputs(float deltaTime,SoundManager& soundManager);

};




#endif //HERO_H
