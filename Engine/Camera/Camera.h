//
// Created by hous on 11/7/24.
//

#ifndef CAMERA_H
#define CAMERA_H



class Camera {
public:
    float fov;
    float aspect;
    void setFov(float);
    void setAspect(float);
    void update();
    void HandleInputs();
    Camera();
};



#endif //CAMERA_H
