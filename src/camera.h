#pragma once

#include "shader.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Camera {
    public:
        void Setup();

        void Use();
        void Set_render_distance(float near, float far);
        void Set_window_size(int width, int height);
        void Set_fov(float newFOV);
        void Set_view(glm::mat4 newView);

        glm::vec2 Get_window_size();

        glm::mat4 mvp;

    private:
        void calc_matrices();

        float fov;
        float renderNear, renderFar;
        int windowWidth, windowHeight;

        glm::mat4 projection;
        glm::mat4 view;
};
