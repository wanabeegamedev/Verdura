//
// Created by hous on 12/25/24.
//

#ifndef ATTACK_H
#define ATTACK_H
#include "../../Engine/ParticleEffect/ParticleManager.h"
#include <string>


class  Attack {
    public:
        double damagePoints{};
        ParticleManager particleManager;
        Particle particlePrototype;
        std::string particlePath;
        Program* program;
        Attack(glm::vec3 position,int facingDirection,
            const std::string& ParticlePath,double _damagePoints,
        Program* _program);
};

inline Attack::Attack(glm::vec3 position,int facingDirection,
                      const std::string &ParticlePath,
                      const double _damagePoints,
                      Program* _program):
particlePrototype(ParticlePath,position,
                          facingDirection,_program)
{
    particlePath = ParticlePath;
    damagePoints = _damagePoints;
    program = _program;
    particlePrototype.setSound("fireSound");
    particlePrototype.load();
    particleManager = ParticleManager();
    particleManager.prepareObjectPool(particlePrototype);
}


#endif //ATTACK_H
