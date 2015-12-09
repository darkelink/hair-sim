#pragma once

#include <glm/glm.hpp>

#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

class Shader {
    public:
        bool Load(std::ifstream file, GLenum type);
        bool Link();
        void Use();
        void Set_uniformm4fv(const GLchar* uniform, glm::mat4 matrix);
        void Set_uniform1f(const GLchar* uniform, GLfloat value);
        void Set_uniform4f(const GLchar* uniform, glm::vec4 value);
        void Set_uniform1i(const GLchar* uniform, GLint value);

        GLuint GetID();

    private:
        std::vector<GLuint> ids;
        GLuint programid;
};
