//
// Created by hous on 12/2/24.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <GLFW/glfw3.h>

#include "../Camera/Camera.h"
#include "error_codes.h"

class Renderer {
    public:
        GLFWwindow* window;
        Camera camera;//TODO, Use this camera

    Renderer()
    {
        window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
        if (window == nullptr)
            exit(WINDOW_INIT_FAILURE);
        camera = Camera();
    }

};
#endif //RENDERER_H
