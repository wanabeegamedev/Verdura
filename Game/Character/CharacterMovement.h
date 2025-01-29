//
// Created by hous on 12/25/24.
//

#ifndef CHARACTERMOVEMENT_H
#define CHARACTERMOVEMENT_H
#include "../../Engine/Mesh/Mesh.h"


class CharacterMovement
{
public:
    int facingDirection;
    void Move(Mesh& Mesh);
};



#endif //CHARACTERMOVEMENT_H
