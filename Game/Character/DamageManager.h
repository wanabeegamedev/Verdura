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
    // need of a physics manager puisque this check is the only physics, 
    bool checkCollision(const glm::vec3 &,const glm::vec3 &) const;

};



#endif //DAMAGEMANAGER_H
