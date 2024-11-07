//
// Created by hous on 11/7/24.
//

#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include "../BaseLight/BaseLight.h"


class DirectionalLight:BaseLight {
public:
    float squareSideLength;
    explicit DirectionalLight(float);

};



#endif //DIRECTIONALLIGHT_H
