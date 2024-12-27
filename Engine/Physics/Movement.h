//
// Created by hous on 12/27/24.
//

#ifndef MOVEMENT_H
#define MOVEMENT_H
#include <glm/glm.hpp>
class Movement
{
public:
    glm::vec3 velocity;
    int facingDirection;
    Movement(glm::vec3 position, glm::vec3 velocity, int facingDirection);
};
#endif //MOVEMENT_H
