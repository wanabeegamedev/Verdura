//
// Created by hous on 11/27/24.
//

#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <stb_image.h>
class OBJLoader {
public:
    OBJLoader() = default;

    static bool Load(const std::string& filename,
              std::vector<glm::vec3>& outVertices,
              std::vector<glm::vec3>& outNormals,
              std::vector<glm::vec2>& outTexCoords,
              std::vector<unsigned int>& outIndices
              ) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open the OBJ file: " << filename << std::endl;
            return false;
        }

        std::string line;
        std::vector<glm::vec3> tempVertices;
        std::vector<glm::vec3> tempNormals;
        std::vector<glm::vec2> tempTexCoords;

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string prefix;
            ss >> prefix;

            if (prefix == "v") {
                glm::vec3 vertex;
                ss >> vertex.x >> vertex.y >> vertex.z;
                tempVertices.push_back(vertex);
            }
            else if (prefix == "vt") {
                glm::vec2 texCoord;
                ss >> texCoord.x >> texCoord.y;
                tempTexCoords.push_back(texCoord);
            }
            else if (prefix == "vn") {
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                tempNormals.push_back(normal);
            }
            else if (prefix == "f") {
                unsigned int vIndex[3], tIndex[3], nIndex[3];
                char slash ;
                for (int i = 0; i < 3; ++i) {
                    ss >> vIndex[i] >> slash >> tIndex[i] >> slash >> nIndex[i];
                    outIndices.push_back(vIndex[i] - 1);
                }
            }

        }

        outVertices = tempVertices;
        outNormals = tempNormals;
        outTexCoords = tempTexCoords;
        return true;
    }

    static bool LoadTexture(const std::string& texturePath,
              GLuint& textureID) {
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // OpenGL specifique
        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
            return false;
        }

        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        return true;
    }

};

#endif //OBJLOADER_H
