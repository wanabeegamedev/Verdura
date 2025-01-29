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
#include "../Engine/Camera/Camera.h"

constexpr glm::vec3 movementRotate = glm::vec3(.0f, 1.f, .0f);
inline int stride = 8 * sizeof(float);
class OBJMesh : public Mesh
{
public:
    explicit OBJMesh(const char * filename):Mesh(), currentDataPath(filename) {
        meshType = MeshType::OBJ;
        model = glm::mat4(1.0f);
        rotate(glm::radians(90.0f*(float)(facingDirection)), glm::vec3(.0f, 1.0f, 0.0f));
        // en haut on peut mettre que les membre de la class
        // car pour les membres de la base class j'ai appelé le constructeur
    }
    glm::vec3 position;
    int facingDirection{};
    int newFacingDirection{};
    glm::vec3 movementDirection = glm::vec3(.0f, 1.0f, .0f);
    glm::mat4 model;
    bool Initialize(const std::vector<glm::vec3>& vertices,
                    const std::vector<glm::vec3>& normals,
                    const std::vector<glm::vec2>& texCoords,
                    const std::vector<unsigned int>& indices)
    {
        indicesCount = indices.size();
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // Interleave vertex data, NOTE  :  Ram Optimization : I just realized
        // UNDER CONDITION : that no mesh sharing the data will directly change it
        // I can avoid storing them and use a pointer to the cache
        // When max effeciency will be needed
        // Every mesh will then use coeffiecient  in the shaders to change the data
        std::vector<float> interleavedData;
        // OR std::shared_ptr<std::vector<float>> interleavedData; // so the cache will use std::make_shared, #include<memory> for -std=c++11 or lower

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
    // TODO GLuint posLocation = glGetAttribLocation(currentProgram.programID,"position");
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    //  Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Textures
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
        model  = glm::mat4(1.0f);
        model = glm::translate(model, position);
        rotate(glm::radians(90.0f*(float)(newFacingDirection)), movementRotate);

    return true;
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

    void set_position(const glm::vec3& _position) {
        position = _position;
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
    void translate(const glm::vec3& translation) {
        position += translation;
        model = glm::translate(glm::mat4(1.0f), position);
    }

    void rotate(float angleRadians, const glm::vec3& axis) {
        model = glm::rotate(model, angleRadians, axis);
    }

    void scale(const glm::vec3& scaleFactor) {
        model = glm::scale(model, scaleFactor);
    }

    float speed = 10.0f*1;
    void handleInputs(float deltaTime) {
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f); //  Z+
        glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
        bool keyPressed = false;
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
            movementDirection = forward*-1.0f;
            newFacingDirection  = 2;
            keyPressed = true;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
            movementDirection = forward;
            newFacingDirection  = 0;
            keyPressed = true;
        }
        // X
        else if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
            movementDirection = right*-1.0f;
            newFacingDirection  = 3;
            keyPressed = true;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
            movementDirection = right;
            newFacingDirection  = 1;
            keyPressed = true;
        }
        // il faut normalizer movement vector pour contrer le  diagonal speed boost
        if (glm::length(movementDirection) > 0.0f) {
            movementDirection = glm::normalize(movementDirection);
        }
        if (keyPressed) {
            position += movementDirection * speed * deltaTime;
            model  = glm::mat4(1.0f);
            model = glm::translate(model, position);
            rotate(glm::radians(90.0f*(float)(newFacingDirection)), movementRotate);
            facingDirection = newFacingDirection;
            //std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";

        }
    }

    const char* currentDataPath;
    const char* currentTexturePath;
};

#endif //OBJMESH_H
