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
    Hero();
    ~Hero();
    std::string name;
    Stats stats;
    Inventory inventory;
};



#endif //HERO_H
