//
// Created by hous on 12/25/24.
//

#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H
#include <string>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

#include "../Mesh/Mesh.h"

#define NB_PARTICLES 10;
#define LIFETIME 3f;
class Particle {
    public:
    Mesh* mesh;
    std::string pathToFile;
    void load(); // load the billboard, png
    void renderParticle(Program* program, GLuint vboId);
    float lifetime; // particle set to inactive if lifetime hit 0
    glm::vec3 position; // initialize to mesh.position() which is the position of the mesh
    float velocity{0.1f}; //
    glm::mat4 model; // billboard never rotate or scale, it always face camera
    void update();//move up a little bit, also at every update the billboard follow its mesh(the burning character)
};

class ParticleManager {
    public:
    std::vector<Particle> particles;
    void update(float deltaTime); // update particles

};

#endif //PARTICLEMANAGER_H
