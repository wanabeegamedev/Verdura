//
// Created by hous on 12/25/24.
//

#ifndef ATTACK_H
#define ATTACK_H
#include "Ability.h"
#include "../../Engine/ParticleEffect/ParticleManager.h"
#include <string>

#include "../Character/Character.h"

class Attack : public Ability {
    public:
        double damagePoints{};
        ParticleManager particleManager;
        Particle particlePrototype;
        std::string particlePath;
        float attackDuration;
        Program* program;// TODO Specifier que jai utiliser les pointeurs avant de comprendre les ref et que quand j'ai compris j'ai arreté
        Character& Owner;
    Attack(Character& _Owner, const std::string& ParticlePath,double _damagePoints,
        float _attackDuration,Program* _program);
};

inline Attack::Attack(Character& _Owner,
    const std::string &ParticlePath,
    double _damagePoints,float _attackDuration,Program* _program): Ability(),
    Owner(_Owner),
    particlePrototype(Particle(particlePath, attackDuration,Owner.characterMesh.position,
            Owner.characterMesh.facingDirection,program)){
    particlePath = ParticlePath;
    damagePoints = _damagePoints;
    attackDuration = _attackDuration;
    program = _program;
    particleManager = ParticleManager();
    //TODO Create a ton of particles with the prototype
}


#endif //ATTACK_H
