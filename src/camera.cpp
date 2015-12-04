#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

void Camera::Setup() {
    view = glm::lookAt(
            glm::vec3(4,3,3),
            glm::vec3(0,0,0),
            glm::vec3(0,1,0)
            );
    renderNear = 0.1f;
    renderFar = 100.0f;
    windowWidth = 640;
    windowHeight = 480;
    fov = 45.0f;
    calc_matrices();
}

void Camera::Set_render_distance(float near, float far) {
    renderNear = near;
    renderFar = far;
    calc_matrices();
}

void Camera::Set_window_size(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    calc_matrices();
}

void Camera::Set_fov(float newFOV) {
    fov = newFOV;
    calc_matrices();
}

void Camera::Set_view(glm::mat4 newView) {
    view = newView;
    mvp = projection * view * glm::mat4(1.0f);
}

void Camera::calc_matrices() {
    projection = glm::perspective(glm::radians(fov), float(windowWidth)/float(windowHeight), renderNear, renderFar);
    mvp = projection * view * glm::mat4(1.0f);
}
