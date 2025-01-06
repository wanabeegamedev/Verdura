//
// Created by hous on 12/25/24.
//

#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H
#include "glad.h"
#include <stb_image.h>
#include <string>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

#include "../Camera/Camera.h"
#include "../Engine/Shader/program.h"
#include "../Sound/SoundManager.h"


constexpr float PARTICLE_SPEED = .25f; // Je la garde constante
constexpr float MAX_NB_PARTICLES = 100;
constexpr float LIFETIME = 45.f;
inline constexpr float vertices[] = {
    // positions        // texture coords
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, //bas-gauche
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bas-droit
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // haut-droit
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
};

inline constexpr GLuint indices[] = {
    0, 1, 2,
    0, 2, 3
};
// Prototype Pattern
class Particle {
public:
    GLuint VAO{}, VBO{}, EBO{},texture{};
    Program * program;
    std::string pathToFile;
    std::string sound;
    void update(double);//move up a little bit, also at every update the billboard follow its mesh(the burning character)
    void load(); // load the billboard, png, Used Once for every type of particle

    double lifetime; // particle set to inactive if lifetime hit 0
    glm::vec3 position{}; // initialize to mesh.position() which is the position of the mesh
    float velocity{PARTICLE_SPEED}; // particle speed
    int facingDirection;
    bool isActive  = false;
    glm::mat4 model{}; // billboard never rotate, it always face camera
    void translate(const glm::vec3& translation)
    {
        //position += translation;
        model = glm::translate(glm::mat4(1.0f), position); // Update the model matrix
    }
    void setSound(std::string const& _sound);
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
inline void Particle::setSound(std::string const& _sound) {
    sound = _sound;
}

class ParticleManager {
    public:
    std::vector<Particle> particlesObjectPool;
    void update(double); // update particles
    void particleFromPrototype(const Particle& original, glm::vec3 newPosition,
        int newFacingDirection);
    void add(Particle& particle);
    void prepareObjectPool(Particle& particlePrototype);
    void releaseFromObjectPool(
        const glm::vec3& _position,
        int _facingDirection,
        SoundManager&);
    void returnToObjectPool(Particle& particle);
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
inline void Particle::update(double deltaTime)
{
    lifetime -= deltaTime;
    if (lifetime <= 0.0f) {
        isActive = false;
        return;
    }
    position += glm::normalize(movementDirection) * velocity *(float)deltaTime;
    model  = glm::mat4(1.0f);
    model = glm::translate(model, position);
}
inline Particle::Particle(
    const std::string& _path,float _lifetime,
    const glm::vec3 _position,
    int _facingDirection,Program * _program)
{
    pathToFile = _path;
    program = _program;
    lifetime = _lifetime;
    position = _position;
    sound ="";
    model = glm::mat4(1.0f);
    facingDirection = _facingDirection;
    switch (_facingDirection) {
        case 0: // Facing down
            movementDirection = glm::vec3(0.0f, .00f, 0.01f);
        break;
        case 1: // Facing right
            movementDirection = glm::vec3(.01f, 0.0f, 0.0f);
        break;
        case 2: // Facing up
            movementDirection = glm::vec3(0.0f, .0f, -0.01f);
        break;
        case 3: // Facing left
            movementDirection = glm::vec3(-0.01f, 0.0f, 0.0f);
        break;
        default:
            movementDirection = glm::vec3(0.0f); // Default to stationary
    }
}
inline void ParticleManager::particleFromPrototype(const Particle& original,
    glm::vec3 newPosition, int newFacingDirection)
{
    Particle newParticle = original;
    newParticle.isActive = false;
    newParticle.position = newPosition;
    newParticle.facingDirection = newFacingDirection;
    particlesObjectPool.push_back(newParticle);
}
inline void ParticleManager::add(Particle &particle) {
    particlesObjectPool.push_back(particle);
}
inline void ParticleManager::update(double deltaTime) {
    for (Particle& particle : particlesObjectPool) {
        if (particle.isActive)
            particle.update(deltaTime);
    }
}

inline void ParticleManager::prepareObjectPool(Particle &particlePrototype) {
    int size = MAX_NB_PARTICLES;
    add(particlePrototype);
    size--;
    while (size > 0) {
        particleFromPrototype(particlePrototype, particlePrototype.position,
            particlePrototype.facingDirection);
        size--;
    }
}
inline void ParticleManager::releaseFromObjectPool(const glm::vec3& _position,
    const int _facingDirection,SoundManager& soundManager) {
    for (Particle& particle : particlesObjectPool)
    {
        if (!particle.isActive) {
            particle.lifetime = LIFETIME;
            particle.facingDirection = _facingDirection;
            particle.position = _position;//+glm::vec3(2.5f,0.f,2.5f);
            particle.model = glm::mat4(1.0f);
            switch (_facingDirection) {
                case 0: // Facing down
                    particle.movementDirection = glm::vec3(0.0f, .00f, 0.01f);
                break;
                case 1: // Facing right
                    particle.movementDirection = glm::vec3(.01f, 0.0f, 0.0f);
                break;
                case 2: // Facing up
                    particle.movementDirection = glm::vec3(0.0f, .0f, -0.01f);
                break;
                case 3: // Facing left
                    particle.movementDirection = glm::vec3(-0.01f, 0.0f, 0.0f);
                break;
                default:
                    particle.movementDirection = glm::vec3(0.0f);
            }

            //particle.movementDirection = glm::normalize(particle.movementDirection) * 0.01f;

            // Activate particle
            particle.isActive = true;

            // TODO  Peut utiliser ParticleReleaseEvent sur SoundManager,
            // Si le temps suffit
            if (!particle.sound.empty())
                soundManager.playSound(particle.sound);

            // Debug: Log particle properties
            /*std::cout << "Particle Released - Position: ("
                      << particle.position.x << ", "
                      << particle.position.y << ", "
                      << particle.position.z << "), Facing: "
                      << _facingDirection << std::endl;*/
            //particle.renderParticle(1.0f);

            break;
        }
    }
}
inline void ParticleManager::returnToObjectPool(Particle &particle) {
    particle.lifetime = LIFETIME;
    particle.isActive = false;
}


#endif //PARTICLEMANAGER_H
