//
// Created by hous on 11/7/24.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:

    // changent rarement
    float fov;
    float aspectRatio;
    float near;
    float far;

    // changent toujours
    glm::vec3 cameraUp; // change dans un engin volant en first-person
    glm::vec3 cameraFront;
    glm::vec3 cameraRight;
    glm::vec3 cameraPosition;
    float cameraRotationInRadius;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    void update();
    void HandleInputs();

    void set_position(glm::vec3);

    void set_fov(float fov) {
        this->fov = fov;
    }

    void set_aspect_ratio(float aspect_ratio) {
        aspectRatio = aspect_ratio;
    }

    void set_near(float near) {
        this->near = near;
    }

    void set_far(float far) {
        this->far = far;
    }

    void set_camera_up(const glm::vec3 &camera_up) {
        cameraUp = camera_up;
    }

    void set_camera_front(const glm::vec3 &camera_front) {
        cameraFront = camera_front;
    }

    void set_camera_right(const glm::vec3 &camera_right) {
        cameraRight = camera_right;
    }

    void set_camera_position(const glm::vec3 &camera_position) {
        cameraPosition = camera_position;
    }

    void set_camera_rotation_in_radius(float camera_rotation_in_radius) {
        cameraRotationInRadius = camera_rotation_in_radius;
    }

    void set_view_matrix(const glm::mat4 &view_matrix) {
        viewMatrix = view_matrix;
    }

    void set_projection_matrix(const glm::mat4 &projection_matrix) {
        projectionMatrix = projection_matrix;
    }


};



#endif //CAMERA_H
