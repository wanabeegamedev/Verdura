//
// Created by hous on 12/7/24.
//

#ifndef MESH_H
#define MESH_H

class Mesh
{
protected:
    GLuint VAO{}, VBO{}, EBO{};
    unsigned long indicesCount{};
    GLuint currentTextureID{};
    GLuint currentProgramID{};
    std::map<std::string, GLuint> MeshTextures ;
    std::map<std::string, GLuint> MeshPrograms ;
public:
    Mesh();
    void addProgram(GLuint programID, std::string vertexShaderFile, std::string fragmentShaderFile);
    void setProgram(GLuint programID);
    void addTexture(std::string textureFile, GLuint textureID);
    void setTexture(GLuint textureID);

};
#endif //MESH_H
