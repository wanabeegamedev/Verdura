//
// Created by hous on 12/25/24.
//

#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H
#include <stb_image.h>
#include <string>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

#include "../Mesh/Mesh.h"

#define NB_PARTICLES 10;
#define LIFETIME 3f;
inline float vertices[] = {
    // positions        // texture coords
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, //bas-gauche
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bas-droit
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // haut-droit
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
};

inline GLuint indices[] = {
    0, 1, 2,
    0, 2, 3
};
// Prototype Pattern
class Particle {
public:
    GLuint VAO{}, VBO{}, EBO{},texture{};
    Program * program;
    Mesh * mesh;// TODO get position, direction & more
    std::string pathToFile;
    void load(); // load the billboard, png, Used Once for every type of particle
    void renderParticle(double deltaTime) const ;
    float lifetime; // particle set to inactive if lifetime hit 0
    glm::vec3 position{}; // initialize to mesh.position() which is the position of the mesh
    float velocity{450.0f}; // particle speed
    int facingDirection;
    glm::mat4 model{}; // billboard never rotate, it always face camera
    void update(double);//move up a little bit, also at every update the billboard follow its mesh(the burning character)
    void translate(const glm::vec3& translation) {
        position += translation;
        model = glm::translate(glm::mat4(1.0f), position); // Update the model matrix
    }
    glm::vec3 movementDirection{};
    void scale(const glm::vec3& scaleFactor) {
        model = glm::scale(model, scaleFactor);
    }
    void rotate(float angleRadians, const glm::vec3& axis) {
        model = glm::rotate(model, angleRadians, axis);
    }
    Particle(const std::string& _path,float _lifetime, glm::vec3 _position,
    int _facingDirection,Program * _program);
};

class ParticleManager {
    public:
    std::vector<Particle> particles;
    void update(double); // update particles
    void particleFromPrototype(const Particle& original, glm::vec3 newPosition,
        int newFacingDirection);
    void add(Particle& particle);

    void renderParticles(double deltaTime);
};


inline void Particle::load() {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(pathToFile.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture for particle " <<  pathToFile.c_str() << std::endl;
        return;
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

inline void Particle::renderParticle(
    double deltaTime) const {
    if (!program) {
        std::cerr << "Error: Shader program is null." << std::endl;
        return;
    }
    program->bind();
    int textureLocation = program->getUniformLocation("partTexture");
    if (textureLocation == -1) {
        std::cerr << "Error: Uniform 'partTexture' not found." << std::endl;
        return;
    }
    program->setUniform1i("partTexture", 0);
    program->setUniformMat4("model",model);
    program->setUniform1f("uTime", deltaTime);
    if (!glIsTexture(texture)) {
        std::cerr << "Error: Invalid texture ID." << std::endl;
        return;
    }
    glBindTexture(GL_TEXTURE_2D, texture);
    if (!glIsVertexArray(VAO)) {
        std::cerr << "Error: Invalid VAO ID." << std::endl;
        return;
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

inline void Particle::update(double deltaTime) {
    position += movementDirection * velocity*(float)deltaTime*100000.0f;
    model  = glm::mat4(1.0f);
    scale(glm::vec3(0.07f));//  POUR LES 64px x 64px; TODO A parametriser
    model = glm::translate(model, position);
    rotate(glm::radians(90.0f*(float)(facingDirection-1)), glm::vec3(0.0f, .0f, 1.0f));// TODO #define PARTICLE_ROTATION_AXIS
    //std::cout << position.x << " " << position.y << " " << position.z << std::endl;
}

inline Particle::Particle(const std::string& _path,float _lifetime,
     const glm::vec3 _position,
     int _facingDirection,Program * _program)
{
    pathToFile = _path;
    program = _program;
    lifetime = _lifetime;
    position = _position;
    model = glm::mat4(1.0f);
    facingDirection = _facingDirection;
    switch (_facingDirection) {
        case 0 :
            movementDirection = glm::vec3(0.0f,-0.01f,0.0f);
            break;
        case 1 :
            movementDirection = glm::vec3(0.01f,0.0f,0.0f);
            break;
        case 2 :
            movementDirection = glm::vec3(0.0f,0.01f,0.0f);
        break;
        case 3 :
            movementDirection = glm::vec3(-0.01f,0.0f,0.0f);
        break;
        default: ;
    }
    rotate(glm::radians(90.0f*(float)(_facingDirection-1)), glm::vec3(0.0f, 1.0f, 0.0f));
    //std::cout << movementDirection.x << " " << movementDirection.y << " "<<movementDirection.z<< " " <<std::endl;
}

inline void ParticleManager::particleFromPrototype(const Particle& original,
    glm::vec3 newPosition, int newFacingDirection)
{
    Particle newParticle = original;
    newParticle.position = newPosition;
    newParticle.facingDirection = newFacingDirection;
    newParticle.model = glm::translate(glm::mat4(1.0f), newPosition);
    particles.push_back(newParticle);
}
inline void ParticleManager::add(Particle &particle) {
    particles.push_back(particle);
}

// TODO deltaTime is a double
inline void ParticleManager::update(double deltaTime) {
    for (Particle& particle : particles) {
        particle.update(deltaTime);
    }
}
inline void ParticleManager::renderParticles(double deltaTime) {
    for (Particle& particle : particles) {
        particle.renderParticle(deltaTime);
    }
}

#endif //PARTICLEMANAGER_H
