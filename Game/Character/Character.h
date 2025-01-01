//
// Created by hous on 12/25/24.
//

#ifndef CHARACTER_H
#define CHARACTER_H
#include <vector>
#include <string>

#include "Stats.h"
#include "../../Engine/Mesh/OBJMesh.h"

class Character{
public:
    virtual ~Character() = default;

    OBJMesh characterMesh;
    Character(): characterMesh(nullptr) {
    }
    virtual void attackSuccessful();
    virtual void attackReceived();
};



#endif //CHARACTER_H
