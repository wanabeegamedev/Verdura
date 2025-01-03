//
// Created by hous on 12/25/24.
//

#ifndef ATTACK_H
#define ATTACK_H
#include "Ability.h"
#include "../../Engine/ParticleEffect/ParticleManager.h"
#include <string>

//#include "../../Engine/Mesh/OBJMesh.h"
//#include "../Character/Character.h"

class  Attack : public Ability {
    public:
        double damagePoints{};
        ParticleManager particleManager;
        Particle particlePrototype;
        std::string particlePath;
        float attackDuration;
        Program* program;// TODO Specifier que jai utiliser les pointeurs avant de comprendre les ref et que quand j'ai compris j'ai arreté
        //Character& Owner;
        Attack(/*Character& _owner,*/glm::vec3 position,int facingDirection,
            const std::string& ParticlePath,double _damagePoints,
        float _attackDuration,Program* _program);//_Owner est un identifiant reservé
};

inline Attack::Attack(/*Character& _owner,*/
                      glm::vec3 position,int facingDirection,
                      const std::string &ParticlePath,
                      const double _damagePoints,float _attackDuration,
                      Program* _program):
Ability(),
particlePrototype(ParticlePath, _attackDuration,position,
                          facingDirection,_program)
/*Owner(_owner)*/
{
    particlePath = ParticlePath;
    damagePoints = _damagePoints;
    attackDuration = _attackDuration;
    program = _program;
    particlePrototype.setSound("fireSound");//TODO
    particlePrototype.load();
    particleManager = ParticleManager();
    particleManager.prepareObjectPool(particlePrototype);
    //TODO Create a ton of particles with the prototype
}


#endif //ATTACK_H
