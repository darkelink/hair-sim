#include "controller.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>
#include <vector>

#include <stdio.h>

float Controller::radius = 5.0f;
std::string Controller::entry;

Controller::Controller(GLFWwindow* window, Params* params, Scene* scene) {
    position  = glm::vec3(0.0f,0.0f,0.0f);
    win = window;
    settings = params;
    sce = scene;
    theta = 0;
    phi = 0;
}

void Controller::Set_camera(Camera* cam) {
    camera = cam;
}

void Controller::Get_input() {
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, GL_TRUE);
    }
    if (glfwGetKey(win, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
        if (entry.length() > 0) {
            entry.clear();
            glfwSetWindowTitle(win, "Particles");
        }
    }
    if (glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS) {
        if (entry.length() > 0) {
            if (editingValue.length() > 0) {
                try{
                    float newValue = stof(entry, nullptr);
                    settings->Modify(editingValue, newValue);
                } catch ( ... ) {
                    glfwSetWindowTitle(win, "Not a number.");
                }
                editingValue.clear();
                reload_scene();
            }
            else {
                if (entry == "list") {
                    std::cout << std::endl << "Current editable settings:" << std::endl;
                    std::vector<std::string> list = settings->Get_params();
                    for (auto i : list) {
                        std::cout << i << " : "<< settings->Get_value(i) << std::endl;
                    }
                } else if (!settings->Exists(entry)) {
                    glfwSetWindowTitle(win, "No such setting.");
                } else {
                    editingValue = entry;
                    glfwSetWindowTitle(win, "Enter new value");
                }
            }
            entry.clear();
        }
    }
}

void Controller::Update_view(float dt) {

    double xpos, ypos;
    glfwGetCursorPos(win, &xpos, &ypos);

    if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        theta += speed * (xpos - oldx);
        phi   += speed * (ypos - oldy);

        position = glm::vec3(
                radius * cos(phi) * sin(theta),
                radius * sin(phi) * sin(theta),
                radius *            cos(theta)
                );

        glm::mat4 mvp = glm::lookAt(position, glm::vec3(0,0,0),
                glm::vec3(0,1,0));

        camera->Set_view(mvp);
    }

    oldx = xpos;
    oldy = ypos;
}

void Controller::Zoom(GLFWwindow* window, double x, double y) {
    radius -= scrollSpeed * (float)y;
}

void Controller::Text(GLFWwindow* window, unsigned int codepoint) {
    entry.push_back((char)codepoint);
    glfwSetWindowTitle(window, entry.c_str());
}

void Controller::reload_scene() {
    sce->Gen_hair(
        (int)settings->Get_value("strands"),
        (int)settings->Get_value("segments"),
        settings->Get_value("length"));
}
