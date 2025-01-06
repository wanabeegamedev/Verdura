//
// Created by hous on 1/6/25.
//

#ifndef MATH_HELPER_H
#define MATH_HELPER_H
#include <vector>
#include <glm/vec3.hpp>
inline std::vector<glm::vec3> generateEnemyPositions(float xMin, float xMax, float zMin, float zMax, int rows, int cols) {
    std::vector<glm::vec3> positions;
    float xStep = (xMax - xMin) / (cols - 1);
    float zStep = (zMax - zMin) / (rows - 1);

    // TODO remove bricolage
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            float x = xMin + col * xStep + (row%3==0?0:xStep/2)*(col%2==0?-1:1);
            float z = (zMin + row * zStep)*(col%2==0?-1:1);
            z = (col%3==0?0:z);
            z = ((col-1)%3!=0?z+(col%2==0?2:-1):z);
            positions.emplace_back(x, 0.0f, z);
        }
    }
    return positions;
}

#endif //MATH_HELPER_H
