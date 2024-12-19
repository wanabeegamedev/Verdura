//
// Created by hous on 12/17/24.
//

#ifndef MESHCACHE_H
#define MESHCACHE_H
#include <vector>
#include <glm/glm.hpp>

//TODO : Not enough time to end the cache now
class  MeshCache {
    public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
};
#endif //MESHCACHE_H
