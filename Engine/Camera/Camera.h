//
// Created by hous on 11/7/24.
//


#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    // Camera attributes
    float fov;
    float aspectRatio;
    float near;
    float far;
    glm::vec3 cameraUp;
    glm::vec3 cameraFront;
    glm::vec3 cameraRight;
    glm::vec3 cameraPosition;
    float cameraRotationInRadius;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    float speed;
    float sensitivity;

    Camera(glm::vec3 startPosition, glm::vec3 startUp)
        : cameraPosition(startPosition), cameraUp(startUp),
          speed(2.5f), sensitivity(0.1f), fov(45.0f), near(0.1f), far(100.0f) {
        updateCameraVectors();
    }
    void update() {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);
        viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    }
    void HandleInputs(GLFWwindow *window, float deltaTime) {
        float velocity = speed * deltaTime;

        /*
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            cameraPosition += cameraFront * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPosition -= cameraFront * velocity;
        */
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            cameraPosition -= cameraRight * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPosition += cameraRight * velocity;
    }
    void set_position(glm::vec3 newPosition) {
        cameraPosition = newPosition;
    }

    void set_fov(float newFov) {
        fov = newFov;
    }
    void set_aspect_ratio(float newAspectRatio) {
        aspectRatio = newAspectRatio;
    }
    void set_near(float newNear) {
        near = newNear;
    }
    void set_far(float newFar) {
        far = newFar;
    }
    void set_camera_up(const glm::vec3& newUp) {
        cameraUp = newUp;
        updateCameraVectors();
    }
    void set_camera_front(const glm::vec3& newFront) {
        cameraFront = newFront;
        updateCameraVectors();
    }
    void set_camera_right(const glm::vec3& newRight) {
        cameraRight = newRight;
        updateCameraVectors();
    }
    void set_camera_position(const glm::vec3& newPosition) {
        cameraPosition = newPosition;
    }
    void set_view_matrix(const glm::mat4& newViewMatrix) {
        viewMatrix = newViewMatrix;
    }
    void set_projection_matrix(const glm::mat4& newProjectionMatrix) {
        projectionMatrix = newProjectionMatrix;
    }

private:
    void updateCameraVectors() {
        glm::vec3 front;
        cameraFront = glm::normalize(front);
        cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
    }
};

#endif // CAMERA_H

