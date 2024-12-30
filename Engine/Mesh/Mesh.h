//
// Created by hous on 12/7/24.
//

#ifndef MESH_H
#define MESH_H
#include <map>

#include "../Shader/program.h"

enum MeshType {
    OBJ = 0,
};
class Mesh
{
public:

    GLuint VAO{}, VBO{}, EBO{};
    unsigned long indicesCount{};
    GLuint currentTextureID{};
    std::string currentProgramName{};
    std::map<std::string, GLuint> MeshTextures ;
    std::map<std::string, Program*> MeshPrograms ;


    Mesh()= default;
    MeshType meshType{};
    void addProgram(Program* _program){
        MeshPrograms[_program->program_name()] = _program;
    }
    void addTexture(GLuint textureID,const std::string& _textureName) {
        MeshTextures[_textureName] = textureID;
    }
    void setCurrentProgram(const std::string& programName) {
        currentProgramName = programName;
    }
    void setCurrentTexture(std::string const& textureName) {
        currentTextureID = MeshTextures[textureName];
    }
};
#endif //MESH_H
