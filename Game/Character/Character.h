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


    OBJMesh characterMesh;
    Character(): characterMesh(nullptr) {
    }
    virtual void attackSuccessful() = 0;
    virtual void attackReceived(float) = 0;

    virtual ~Character() = default;
};



#endif //CHARACTER_H
