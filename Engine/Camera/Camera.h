//
// Created by hous on 11/7/24.
//


#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>

#include <imgui.h>
#define INITIAL_SPEED 500000.0f
#define INITIAL_NEAR 0.1f
#define INITIAL_SENSITIVITY 0.1f
#define INITIAL_FOV 60.0f
#define INITIAL_FAR 100.0f
#define INITIAL_NEAR 0.1f
#define INITIAL_POS glm::vec3(5.0f, 5.0f, 12.0f)
#define INITIAL_FRONT glm::vec3(-1.0f, -1.0f, -1.0f)// Vers l'origine
#define INITIAL_UP glm::vec3(0.0f, 1.0f, 0.0f)
class Camera {

    float fov;
    float aspectRatio;
    float near;
    float far;



    float cameraRotationInRadius{};
    float speed;
    float sensitivity;
public:
    glm::vec3 cameraUp;
    glm::vec3 cameraFront;
    glm::vec3 cameraRight{};
    glm::vec3 cameraPosition;
    glm::mat4 viewMatrix{};
    glm::mat4 projectionMatrix{};


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
    float aspect_ratio() const {
        return aspectRatio;
    }
    glm::vec3 camera_up() const {
        return cameraUp;
    }
    glm::vec3 camera_front() const {
        return cameraFront;
    }
    glm::vec3 camera_right() const {
        return cameraRight;
    }
    glm::vec3 camera_position() const {
        return cameraPosition;
    }
    glm::mat4 view_matrix() const {
        return viewMatrix;
    }
    glm::mat4 projection_matrix() const {
        return projectionMatrix;
    }
    float camera_rotation_in_radius() const {
        return cameraRotationInRadius;
    }

    void HandleInputs(GLFWwindow *window, double deltaTime) {
        ImGuiIO& io = ImGui::GetIO(); // Get the ImGui input/output state

        float velocity = speed*deltaTime;

        // Check if ImGui does not want to capture the keyboard
        //if (!io.WantCaptureKeyboard)

            // ImGui key codes match GLFW key codes
            if (ImGui::IsKeyPressed(ImGuiKey_Q))

            {
                cameraPosition -= cameraRight * velocity;
            }
            if (ImGui::IsKeyPressed(ImGuiKey_D))

            {
                cameraPosition += cameraRight * velocity;
            }
            // Update camera matrices
            update();

    }
    void update() {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);
        viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    }
    void updateCameraVectors() {
        cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
    }

    Camera():
      speed(INITIAL_SPEED),
      sensitivity(INITIAL_SENSITIVITY),
      fov(INITIAL_FOV),
      near(INITIAL_NEAR),
      far(INITIAL_FAR),
      cameraFront(INITIAL_FRONT),
      cameraPosition(INITIAL_POS),
      cameraUp(INITIAL_UP),
    aspectRatio(float(1280.0/720.0)) // TODO, utiliser des variables
    /*
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);
     */
    {
        updateCameraVectors();
        update();
        //std::cout << glm::to_string(cameraFront) << std::endl << glm::to_string(projection_matrix()) << std::endl;
    }
};

#endif // CAMERA_H

