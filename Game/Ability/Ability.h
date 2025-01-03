//
// Created by hous on 12/25/24.
//

#ifndef ABILITY_H
#define ABILITY_H
#include "../Inventory/Item.h"


class Ability:public Item {
public:
    double manaCost{};
    //TODO Revoir ici
    Ability(){};
};



#endif //ABILITY_H
