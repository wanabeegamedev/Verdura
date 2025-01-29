//
// Created by hous on 12/26/24.
//

#include "DamageManager.h"


bool DamageManager::checkCollision(const glm::vec3 &point,const glm::vec3 &position) const {
    float collisionRadius = 1.2f; //Pour toutes les particules
    return glm::distance(point, position) < collisionRadius;
}