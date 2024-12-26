//
// Created by hous on 12/25/24.
//

#ifndef CHARACTER_H
#define CHARACTER_H
#include <vector>

#include "Stats.h"
#include "../../Engine/Mesh/Mesh.h"
#include "../Ability/DefenseStrategy.h"


class Character {
public:
    Mesh characterMesh;
    std::string name;
    // TODO Movement
    Stats stats;
    Character();
};



#endif //CHARACTER_H
