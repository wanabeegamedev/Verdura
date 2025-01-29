//
// Created by hous on 1/6/25.
//

#ifndef OBJMESHFLYWEIGHT_H
#define OBJMESHFLYWEIGHT_H
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "imgui.h"

constexpr glm::vec3 rotationAxis = glm::vec3(.0f, 1.f, .0f);
class OBJMeshFlyWeight {
public:
    glm::vec3 position{};
    int facingDirection{};
    int newFacingDirection{};
    glm::vec3 movementDirection = glm::vec3(.0f, 1.0f, .0f);
    glm::mat4 model{};
    float speed = 10.0f*1;
    void translate(const glm::vec3& translation) {
        position += translation;
        model = glm::translate(glm::mat4(1.0f), position);
    }
    void rotate(float angleRadians, const glm::vec3& axis) {
        model = glm::rotate(model, angleRadians, axis);
    }
    inline explicit OBJMeshFlyWeight(const glm::vec3& _position) {
        position = _position;
        model = glm::translate(model, position);
        rotate(glm::radians(90.0f*(float)(newFacingDirection)), rotationAxis);
    }
};
#endif //OBJMESHFLYWEIGHT_H
