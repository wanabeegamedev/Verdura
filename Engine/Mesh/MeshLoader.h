//
// Created by hous on 12/7/24.
//

#ifndef MESHLOADER_H
#define MESHLOADER_H

#include<vector>
#include<string>
#include<fstream>
#include <glm/glm.hpp>

#include "Mesh.h"

class MeshLoader
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    const char* currentDataPath;
    public:
    void switchMeshData(Mesh& mesh,const char* dataPath);
};
#endif //MESHLOADER_H
