//
// Created by hous on 11/27/24.
//

#ifndef OBJMESH_H
#define OBJMESH_H

#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "Mesh.h"
int stride = 8 * sizeof(float);
class OBJMesh : public Mesh
{
public:
    explicit OBJMesh(const char * filename):Mesh(), currentDataPath(filename) {
        meshType = MeshType::OBJ;
        // en haut on peut mettre que les membre de la class
        // car pour les membres de la base class j'ai appelé le constructeur
    }
    glm::vec3 position;
    bool Initialize(const std::vector<glm::vec3>& vertices,
                    const std::vector<glm::vec3>& normals,
                    const std::vector<glm::vec2>& texCoords,
                    const std::vector<unsigned int>& indices)
    {
        indicesCount = indices.size();
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // Interleave vertex data
        std::vector<float> interleavedData;
        for (size_t i = 0; i < vertices.size(); ++i)
            {
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

    //  Positions
    //  GLuint posLocation = glGetAttribLocation(currentProgram.programID,"position");
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    //  Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Textures
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind VAO
    glBindVertexArray(0);

    return true;
}

    // TODO Move to Renderer
    void Render() const
    {
        //TODO std::string programName ="Basic";
        //TODO glUseProgram(MeshPrograms.at(programName));
        if (currentTextureID != 0)
        {
            glBindTexture(GL_TEXTURE_2D, currentTextureID);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    ~OBJMesh() {
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (VBO != 0) glDeleteBuffers(1, &VBO);
        if (EBO != 0) glDeleteBuffers(1, &EBO);
        if (currentTextureID != 0) glDeleteTextures(1, &currentTextureID);
    }

    GLuint &current_texture_id()
    {
        return currentTextureID;
    }

    void set_position(glm::vec3&& position) {
        position = position;
    };
    GLuint current_program_id()
    {
        return currentProgramID;
    };

    const char * current_data_path() const {
        return currentDataPath;
    }
    void set_current_data_path(const char *current_data_path) {
        currentDataPath = current_data_path;
    }
    const char * current_texture_path() const {
        return currentTexturePath;
    }
    void set_current_texture_path(const char *current_texture_path) {
        currentTexturePath = current_texture_path;
    }
    const char* currentDataPath;
    const char* currentTexturePath;
};

#endif //OBJMESH_H
