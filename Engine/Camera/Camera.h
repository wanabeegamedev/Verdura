//
// Created by hous on 11/7/24.
//
/*isometric
#define INITIAL_POS glm::vec3(10.0f, 10.0f, 10.0f) // Camera is above and offset diagonally
#define INITIAL_FRONT glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - INITIAL_POS) // Point at origin
#define INITIAL_UP glm::vec3(0.0f, 1.0f, 0.0f) // Y-axis remains "up"
*/
/*
facing
#define INITIAL_POS glm::vec3(.0f, .0f, 20.0f)
#define INITIAL_FRONT  glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - INITIAL_POS)
#define INITIAL_UP glm::vec3(0.0f, 1.0f, 0.0f)
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <imgui.h>


#define INITIAL_SPEED 500000.0f
#define INITIAL_NEAR 0.1f
#define INITIAL_SENSITIVITY 0.08f
#define INITIAL_FOV 60.0f
#define INITIAL_FAR 100.0f
#define INITIAL_NEAR 0.1f


//top down
#define INITIAL_POS glm::vec3(0.0f, 20.0f, 0.0f) // Camera is 20 units above the origin
#define INITIAL_FRONT glm::vec3(0.0f, -1.0f, 0.0f) // Camera looks straight down
#define INITIAL_UP glm::vec3(0.0f, 0.0f, -1.0f)   // Z-axis is "up" for this perspective



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
    void handleInputs(double deltaTime) {
        ImGuiIO& io = ImGui::GetIO();

        float velocity = speed * static_cast<float>(deltaTime);

        // Movement directions for top-down camera
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f); // Forward in top-down view (positive Z)
        glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);   // Right in top-down view (positive X)

        if (!io.WantCaptureKeyboard) {
            // Check movement key inputs
            if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
                cameraPosition -= right * velocity; // Move left (along the X-axis)
            }
            if (ImGui::IsKeyPressed(ImGuiKey_D)) {
                cameraPosition += right * velocity; // Move right (along the X-axis)
            }
            if (ImGui::IsKeyPressed(ImGuiKey_Z)) {
                cameraPosition += forward * velocity; // Move forward (along the Z-axis)
            }
            if (ImGui::IsKeyPressed(ImGuiKey_S)) {
                cameraPosition -= forward * velocity; // Move backward (along the Z-axis)
            }
        }
    }

    void update() {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, near, far); //TODO
        //projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near, far);
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
    {
        updateCameraVectors();
        update();
    }
};

#endif // CAMERA_H

