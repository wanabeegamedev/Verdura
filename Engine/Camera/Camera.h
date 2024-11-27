//
// Created by hous on 11/7/24.
//


#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h> // For GLFW key inputs

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

    float speed; // Speed of movement
    float sensitivity; // Sensitivity for mouse movement

    float yaw;
    float pitch;

    // Camera constructor
    Camera(glm::vec3 startPosition, glm::vec3 startUp, float startYaw, float startPitch)
        : cameraPosition(startPosition), cameraUp(startUp), yaw(startYaw), pitch(startPitch),
          speed(2.5f), sensitivity(0.1f), fov(45.0f), near(0.1f), far(100.0f) {
        updateCameraVectors();
    }

    // Update the view and projection matrices
    void update() {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);
        viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    }

    // Handle user input (keyboard and mouse)
    void HandleInputs(GLFWwindow *window, float deltaTime) {
        float velocity = speed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPosition += cameraFront * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPosition -= cameraFront * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPosition -= cameraRight * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPosition += cameraRight * velocity;
    }

    // Set the camera position (manually set position)
    void set_position(glm::vec3 newPosition) {
        cameraPosition = newPosition;
    }

    // Set the field of view (fov) for the camera
    void set_fov(float newFov) {
        fov = newFov;
    }

    // Set aspect ratio (used for the projection matrix)
    void set_aspect_ratio(float newAspectRatio) {
        aspectRatio = newAspectRatio;
    }

    // Set near plane distance for projection
    void set_near(float newNear) {
        near = newNear;
    }

    // Set far plane distance for projection
    void set_far(float newFar) {
        far = newFar;
    }

    // Set the camera's "up" vector
    void set_camera_up(const glm::vec3& newUp) {
        cameraUp = newUp;
        updateCameraVectors();
    }

    // Set the camera's "front" vector (direction of view)
    void set_camera_front(const glm::vec3& newFront) {
        cameraFront = newFront;
        updateCameraVectors();
    }

    // Set the camera's "right" vector (used for lateral movement)
    void set_camera_right(const glm::vec3& newRight) {
        cameraRight = newRight;
        updateCameraVectors();
    }

    // Set the camera's position
    void set_camera_position(const glm::vec3& newPosition) {
        cameraPosition = newPosition;
    }

    // Set the camera's rotation (yaw and pitch)
    void set_camera_rotation_in_radius(float newYaw, float newPitch) {
        yaw = newYaw;
        pitch = newPitch;
        updateCameraVectors();
    }

    // Set the view matrix (transformation for camera position and rotation)
    void set_view_matrix(const glm::mat4& newViewMatrix) {
        viewMatrix = newViewMatrix;
    }

    // Set the projection matrix (perspective or orthographic)
    void set_projection_matrix(const glm::mat4& newProjectionMatrix) {
        projectionMatrix = newProjectionMatrix;
    }

private:
    // Update the camera's front, right, and up vectors based on yaw and pitch
    void updateCameraVectors() {
        // Calculate the new front vector
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);

        // Recalculate the right and up vectors
        cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));  // Right vector
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront)); // Recalculate the up vector
    }
};

#endif // CAMERA_H

