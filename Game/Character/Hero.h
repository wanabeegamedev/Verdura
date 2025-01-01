//
// Created by hous on 12/30/24.
//

#ifndef HERO_H
#define HERO_H
#include "Character.h"
#include "../Inventory/Inventory.h"


class Hero:public Character{
    public:

    //TODO std::vector<WarriorClass> HeroClasses
    Hero(OBJMesh& _characterMesh,Stats& _stats, Inventory& _inventory,
        const std::string& _name);

    void attackSuccessful() override;
    void attackReceived(float damage) override;

    ~Hero();
    std::string name;
    Stats stats;
    Inventory inventory;
};




#endif //HERO_H
