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
#include "OBJLoader.h"
#include "OBJMesh.h"
#include "../Shader/program.h"
#include "../Shader/shader.h"

class MeshLoader
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    const char*currentDataPath = nullptr;

    void resetLoader() {
        vertices.clear();
        normals.clear();
        texCoords.clear();
        indices.clear();
        currentDataPath = nullptr;
    }
    public:

    //  Utiliser Mesh Data en cache
    // TODO Trouver un moyen d'abstraire cette méthode ?
    inline void switchOBJMeshData(OBJMesh &mesh, const char *dataPath)
    {
        mesh.currentDataPath = dataPath;
        if (mesh.meshType == MeshType::OBJ)
            LoadObjMesh(mesh);
    };

    [[maybe_unused]] // UNUSED , trop tard
    static inline void LoadProgram(
    Mesh &mesh,
    const std::string &vertexShaderFilePath,
    const std::string &fragmentShaderFilePath,
    const std::string &programName)
    {
        Shader vShader(vertexShaderFilePath, GL_VERTEX_SHADER);
        Shader fShader(fragmentShaderFilePath, GL_FRAGMENT_SHADER);
        //auto program = std::make_shared<Program>(vShader.shader_id(), fShader.shader_id());
        auto program = new Program(vShader.shader_id(), fShader.shader_id());
        program->setName(programName);
        mesh.addProgram(program);
    }

    inline void LoadTexture(const std::string& textureFile, GLuint textureID)
    {
        //  TODO
    };
    void LoadObjMesh(OBJMesh& mesh /*,const char* meshName*/)
    {
        /*if (MeshLoader::GetOBJMesh(meshName))//Mesh is in the cache
        {
            //initlialise
            MeshLoader::Retrieve4fv(vertices, normals, texCoords, indices);
            mesh.Initialize(vertices, normals, texCoords, indices);
            resetLoader();
            return;
        }*/
        if (OBJLoader::Load(
        mesh.currentDataPath,
        vertices, normals, texCoords, indices))
        {
            if (OBJLoader::LoadTexture(mesh.currentTexturePath,
            mesh.current_texture_id()))
                mesh.Initialize(vertices, normals, texCoords, indices);
        }
        resetLoader();
    };
};
#endif //MESHLOADER_H
