#pragma once

#include "camera.h"
#include "params.h"
#include "scene.h"

#include <GLFW/glfw3.h>

#include <string>

class Controller {
    public:
        Controller(GLFWwindow* window, Params* params, Scene* scene);

        void Set_camera(Camera* cam);
        void Get_input();
        void Update_view(float dt);


        static void Zoom(GLFWwindow* window, double x, double y);
        static void Text(GLFWwindow* window, unsigned int codepoint);
    private:
        Camera* camera;
        GLFWwindow* win;
        Params* settings;
        Scene* sce;

        glm::vec3 position;

        double oldx, oldy;
        float phi, theta;

        std::string editingValue = "";

        // static because callbacks are stupid
        static float radius;
        static std::string entry;

        static constexpr float speed = 0.005f;
        static constexpr float PI = 3.14159;
        static constexpr float TAU = PI*2;
        static constexpr float scrollSpeed = 0.1f;

        void reload_scene();
};
