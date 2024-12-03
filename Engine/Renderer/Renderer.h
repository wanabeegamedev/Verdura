//
// Created by hous on 12/2/24.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <GLFW/glfw3.h>

#include "../Camera/Camera.h"

class Renderer {
    public:
        GLFWwindow* window;
        Camera camera;

    Renderer()
    {
        //TODO DEAL WITH WINDOW CREATION
        camera = Camera();
    }

};
#endif //RENDERER_H
