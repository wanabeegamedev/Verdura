//
// Created by hous on 11/27/24.
//

#ifndef OBJMESH_H
#define OBJMESH_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
int stride = 8 * sizeof(float);
class OBJMesh {
public:
    OBJMesh() : VAO(0), VBO(0), EBO(0), textureID(0) {}

    bool Initialize(const std::vector<glm::vec3>& vertices,
                const std::vector<glm::vec3>& normals,
                const std::vector<glm::vec2>& texCoords,
                const std::vector<unsigned int>& indices) {
    indicesCount = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Interleave vertex data
    std::vector<float> interleavedData;

    for (size_t i = 0; i < vertices.size(); ++i) {
        // positions
        interleavedData.push_back(vertices[i].x);
        interleavedData.push_back(vertices[i].y);
        interleavedData.push_back(vertices[i].z);

        // normals
        interleavedData.push_back(normals[i].x);
        interleavedData.push_back(normals[i].y);
        interleavedData.push_back(normals[i].z);

        // textures
        interleavedData.push_back(texCoords[i].x);
        interleavedData.push_back(texCoords[i].y);
    }

    // VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, interleavedData.size() * sizeof(float), interleavedData.data(), GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    GLsizei stride = (3 + 3 + 2) * sizeof(float);  // Position (3), Normal (3), Texture coords (2)

    //  positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    //  normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // textures
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // unbind VAO
    glBindVertexArray(0);

    return true;
}


    void Render() const
    {
        if (textureID != 0)
        {
            glBindTexture(GL_TEXTURE_2D, textureID);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    ~OBJMesh() {
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (VBO != 0) glDeleteBuffers(1, &VBO);
        if (EBO != 0) glDeleteBuffers(1, &EBO);
        if (textureID != 0) glDeleteTextures(1, &textureID);
    }

    GLuint VAO, VBO, EBO, textureID;
    GLsizei indicesCount{};


};

#endif //OBJMESH_H
