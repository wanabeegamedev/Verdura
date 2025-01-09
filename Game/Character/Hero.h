//
// Created by hous on 12/30/24.
//

#ifndef HERO_H
#define HERO_H
#include "Character.h"
#include "WarriorClass.h"

class Hero:public Character{
    public:

    std::vector<WarriorClass> HeroClasses;
    Hero(OBJMesh* _characterMesh,Stats& _stats,
        const std::string& _name);

    void attackSuccessful() override;
    void attackReceived(float damage) override;

    ~Hero() override;
    std::string name;
    Stats& stats;

};




#endif //HERO_H
