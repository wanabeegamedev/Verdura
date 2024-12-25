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
        model = glm::mat4(1.0f);
        // en haut on peut mettre que les membre de la class
        // car pour les membres de la base class j'ai appelé le constructeur
    }
    glm::vec3 position;
    double yaw{};
    int facingDirection{};
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

    // Unbind VAO
    glBindVertexArray(0);

    return true;
}

    // TODO Move to Renderer
    void Render(const Camera& camera) const
    {
        //TODO std::string programName ="Basic";
        //TODO glUseProgram(MeshPrograms.at(programName));
        if (currentTextureID != 0)
        {
            glBindTexture(GL_TEXTURE_2D, currentTextureID);
        }
        MeshPrograms.at(currentProgramName)->bind();
        MeshPrograms.at(currentProgramName)->setUniform3f("offsetPos", position);// TODO = GET RID
        MeshPrograms.at(currentProgramName)->setUniform1i("texture1", 0);
        MeshPrograms.at(currentProgramName)->setUniformMat4("model",model);
        MeshPrograms.at(currentProgramName)->setUniformMat4("view",camera.viewMatrix);
        MeshPrograms.at(currentProgramName)->setUniformMat4("projection",camera.projectionMatrix);
        // TODO Offseter la Position ICI
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
        model = glm::translate(glm::mat4(1.0f), position); // Update the model matrix
    }

    void rotate(float angleRadians, const glm::vec3& axis) {
        model = glm::rotate(model, angleRadians, axis);
    }

    void scale(const glm::vec3& scaleFactors) {
        model = glm::scale(model, scaleFactors);
    }
    void faceDirection(const glm::vec3& targetPosition) {
        glm::vec3 direction = glm::normalize(targetPosition - position);

        // angle between the forward vector (assume -Z by default) and the direction
        glm::vec3 forward = glm::vec3(.0f, 0.0f, -1.0f);
        float dot = glm::dot(forward, glm::vec3(direction.x, 0.0f, direction.z));
        float angle = glm::acos(glm::clamp(dot, -1.0f, 1.0f));

        // rotation axis (cross product of forward and direction vectors)
        glm::vec3 axis = glm::cross(forward, glm::vec3(direction.x, 0.0f, direction.z));
        if (glm::length(axis) < 1e-6) {
            axis = glm::vec3(0.0f, 1.0f, 0.0f); // go to Y-axis if vectors are nearly collinear
        }
        model = glm::rotate(glm::mat4(1.0f), angle, axis);
    }
    float speed = 3.0f*100000;
  /*void handleInputs(float deltaTime) {
        // Movement directions for isometric perspective
        glm::vec3 isoForward = glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)); // Diagonal forward
        glm::vec3 isoRight = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f));    // Diagonal right

        // Movement speed
        const float movementSpeed = 5.0f; // Units per second

        // Initialize movement vector
        glm::vec3 movement = glm::vec3(0.0f);

        // Check key inputs for movement (WASD)
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
            movement -= isoRight; // Move left
        }
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
            movement += isoRight; // Move right
        }
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {

            movement -= isoForward; // Move forward
        }
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {

            movement += isoForward; // Move backward
            //faceDirection(glm::vec3(-1.0f, 1.0f, .0f));
        }

        // Normalize movement vector to prevent diagonal speed boost
        if (glm::length(movement) > 0.0f) {
            movement = glm::normalize(movement);
        }

        // Update character position based on movement
        position += movement * speed * deltaTime;

    }
*/
    void handleInputs(float deltaTime) {
        // Movement directions for top-down perspective
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f); // Forward (positive Z)
        glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);   // Right (positive X)

        // Movement speed
        const float movementSpeed = 5.0f; // Units per second

        // Initialize movement vector
        glm::vec3 movement = glm::vec3(0.0f);

        // Check key inputs for movement (WASD or Arrow keys)
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
            movement -= forward; // Move forward (along the Z-axis)
            facingDirection  = 2;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
            movement += forward; // Move backward (along the Z-axis)
            facingDirection  = 0;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
            movement -= right; // Move left (along the X-axis)
            facingDirection  = 2;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
            movement += right; // Move right (along the X-axis)
        }

        // Normalize movement vector to prevent diagonal speed boost
        if (glm::length(movement) > 0.0f) {
            movement = glm::normalize(movement);
        }

        // Update character position based on movement
        position += movement * speed * deltaTime;

        // Optionally, you could add rotation or other actions based on the movement
    }


    const char* currentDataPath;
    const char* currentTexturePath;
};

#endif //OBJMESH_H
