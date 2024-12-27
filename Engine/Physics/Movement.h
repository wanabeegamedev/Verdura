//
// Created by hous on 12/27/24.
//

#ifndef MOVEMENT_H
#define MOVEMENT_H
#include <glm/glm.hpp>
class Movement
{
public:
    glm::vec3 position{};
    float velocity{};
    int facingDirection{};
    glm::mat4 model{};
    glm::vec3 movementDirection{};
    void translate(const glm::vec3& translation) {
        position += translation;
        model = glm::translate(glm::mat4(1.0f), position);
    }
    void scale(const glm::vec3& scaleFactor) {
        model = glm::scale(model, scaleFactor);
    }
    void rotate(float angleRadians, const glm::vec3& axis) {
        model = glm::rotate(model, angleRadians, axis);
    }
    Movement(glm::vec3 position, glm::vec3 velocity, int facingDirection);
};
#endif //MOVEMENT_H
