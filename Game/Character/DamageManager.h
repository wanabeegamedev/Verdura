//
// Created by hous on 12/26/24.
//

#ifndef DAMAGEMANAGER_H
#define DAMAGEMANAGER_H
#include "../Ability/DefenseStrategy.h"
#include <vector>
#include <glm/glm.hpp>


class DamageManager {
public:
    std::vector<DefenseStrategy> DefensesActives;
    //No need of a physics manager, this check is the only physics
    bool checkCollision(const glm::vec3 &,const glm::vec3 &) const;

};



#endif //DAMAGEMANAGER_H
