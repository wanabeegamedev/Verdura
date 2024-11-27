//
// Created by hous on 11/24/24.
//
#ifndef ASSIMPFBXMESH_H
#define ASSIMPFBXMESH_H

#include <glad/glad.h>
#include <vector>
#include <string>
#include <iostream>
#include <stb_image.h>

class AssimpFbxMesh {
public:
    AssimpFbxMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
        : indicesCount(static_cast<GLsizei>(indices.size())), textureID(0) {
        // Generate and bind buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Positions
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Normals
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // Texture coords
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    ~AssimpFbxMesh()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
        }
    }

    bool LoadTexture(const std::string& texturePath) {
        int width, height, nrChannels;
        //stbi_set_flip_vertically_on_load(true); // Flip vertically for OpenGL
        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
            return false;
        }

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    void Render() const
    {
        //glActiveTexture(GL_TEXTURE0);

        if (textureID != 0)
        {
            glBindTexture(GL_TEXTURE_2D, textureID);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

private:
    GLuint VAO{}, VBO{}, EBO{};
    GLuint textureID;
    GLsizei indicesCount;
};

#endif // ASSIMPFBXMESH_H
