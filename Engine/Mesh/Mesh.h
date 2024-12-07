//
// Created by hous on 12/7/24.
//

#ifndef MESH_H
#define MESH_H
#include <map>
enum MeshType {
    OBJ = 0,
};
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

    Mesh()= default;
    MeshType meshType{};
    void addProgram(GLuint programID,const std::string& _programName) {
        MeshPrograms[_programName] = programID;
    };
    void setTexture(GLuint textureID);

};
#endif //MESH_H
