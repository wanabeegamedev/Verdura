//
// Created by hous on 11/24/24.
//
#ifndef ASSIMPFBXLOADER_H
#define ASSIMPFBXLOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "AssimpFbxMesh.h"
#include <string>
#include <vector>
#include <iostream>

class AssimpFbxLoader {
public:
    AssimpFbxLoader() = default;

    ~AssimpFbxLoader() {
        for (auto& mesh : meshes) {
            delete mesh;
        }
    }

    bool LoadModel(const std::string& filePath, const std::string& texturePath) {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(filePath,
            aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "Assimp Error: " << importer.GetErrorString() << std::endl;
            return false;
        }

        this->texturePath = texturePath; // Store texture path for all meshes
        ProcessNode(scene->mRootNode, scene);
        return true;
    }

    void Render() const
    {
        int i{};
        for (const auto& mesh : meshes) {
            mesh->Render();
            std::cout << i++ << std::endl;
        }
    }

private:
    std::vector<AssimpFbxMesh*> meshes;
    std::string texturePath;

    void ProcessNode(aiNode* node, const aiScene* scene) {
        // Process all meshes in this node
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene));
        }

        // Recursively process all child nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    AssimpFbxMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Process vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            // Position
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);

            // Normal
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);

            // Texture Coordinates
            if (mesh->mTextureCoords[0]) {
                vertices.push_back(mesh->mTextureCoords[0][i].x);
                vertices.push_back(mesh->mTextureCoords[0][i].y);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
            //std::cout << mesh->mTextureCoords[0][i].x << " , "
            //          << mesh->mTextureCoords[0][i].y << std::endl;
        }

        // Process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        // Create AssimpFbxMesh instance
        auto* assimpMesh = new AssimpFbxMesh(vertices, indices);

        // Load texture
        if (!assimpMesh->LoadTexture(texturePath)) {
            std::cerr << "Warning: Failed to load texture for mesh: " << texturePath << std::endl;
        }

        return assimpMesh;
    }
};

#endif // ASSIMPFBXLOADER_H
