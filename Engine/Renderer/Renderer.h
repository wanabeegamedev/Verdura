//
// Created by hous on 12/2/24.
//

#ifndef RENDERER_H
#define RENDERER_H
//#include "GLFW/glfw3.h"
#include "../Engine/ParticleEffect/ParticleManager.h"
#include "../Camera/Camera.h"
#include "error_codes.h"
#include "../Engine/Mesh/OBJMesh.h"
#include "../Mesh/OBJMeshFlyWeight.h"

class Renderer {
    public:
        //GLFWwindow* window;
        Camera camera;

    Renderer()
    {
        //window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
        //if (window == nullptr)
        //    exit(WINDOW_INIT_FAILURE);
        camera = Camera();
    }
     void renderMeshOBJ(const OBJMesh& mesh,float deltaTime) const
    {
        if (mesh.currentTextureID != 0)
        {
            glBindTexture(GL_TEXTURE_2D, mesh.currentTextureID);
        }
        mesh.MeshPrograms.at(mesh.currentProgramName)->bind();
        mesh.MeshPrograms.at(mesh.currentProgramName)->setUniform1f("uTime", deltaTime);
        mesh.MeshPrograms.at(mesh.currentProgramName)->setUniform1i("texture1", 0);
        mesh.MeshPrograms.at(mesh.currentProgramName)->setUniformMat4("model",mesh.model);
        mesh.MeshPrograms.at(mesh.currentProgramName)->setUniformMat4("view",camera.viewMatrix);
        mesh.MeshPrograms.at(mesh.currentProgramName)->setUniformMat4("projection",camera.projectionMatrix);
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indicesCount, GL_UNSIGNED_INT, nullptr);
        //glBindVertexArray(0);
    }
    void renderMeshOBJFromFlyWeight(const OBJMesh& mesh,const OBJMeshFlyWeight& flyweight,float deltaTime) const
    {
        if (mesh.currentTextureID != 0)
        {
            glBindTexture(GL_TEXTURE_2D, mesh.currentTextureID);
        }
        mesh.MeshPrograms.at(mesh.currentProgramName)->bind();
        mesh.MeshPrograms.at(mesh.currentProgramName)->setUniform1f("uTime", deltaTime);
        mesh.MeshPrograms.at(mesh.currentProgramName)->setUniform1i("texture1", 0);
        mesh.MeshPrograms.at(mesh.currentProgramName)->setUniformMat4("model",flyweight.model);
        mesh.MeshPrograms.at(mesh.currentProgramName)->setUniformMat4("view",camera.viewMatrix);
        mesh.MeshPrograms.at(mesh.currentProgramName)->setUniformMat4("projection",camera.projectionMatrix);
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indicesCount, GL_UNSIGNED_INT, nullptr);
    }
     void renderParticle(Particle& particle) const {
        if (!particle.program) {
            std::cerr << "Error: Shader program is null." << std::endl;
            return;
        }
        particle.program->bind();
        int textureLocation = particle.program->getUniformLocation("partTexture");
        if (textureLocation == -1) {
            std::cerr << "Error: Uniform 'partTexture' not found." << std::endl;
            return;
        }
        particle.program->setUniform1i("partTexture", 0);
        particle.program->setUniformMat4("model",particle.model);
        particle.program->setUniformMat4("view",camera.viewMatrix);
        particle.program->setUniformMat4("projection",camera.projectionMatrix);
        /*if (!glIsTexture(particle.texture)) {
            std::cerr << "Error: Invalid texture ID." << std::endl;
            return;
        }*/
        glBindTexture(GL_TEXTURE_2D, particle.texture);
        /*if (!glIsVertexArray(particle.VAO)) {
            std::cerr << "Error: Invalid VAO ID." << std::endl;
            return;
        }*/
        glBindVertexArray(particle.VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
     void renderParticles (ParticleManager& particleManager) const {
        for (Particle& particle : particleManager.particlesObjectPool) {
            if (particle.isActive)
                renderParticle(particle);
        }
    }
    void updateCamera(float deltaTime) {
        camera.handleInputs(deltaTime);
        camera.updateCameraVectors();
        camera.update();
    }

};
#endif //RENDERER_H
