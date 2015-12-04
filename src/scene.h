#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/vec2.hpp>

#include <fstream>
#include <vector>

class Scene {
    public:
        Scene();
        ~Scene();

        bool Load();
        bool Open(std::ifstream file);

        void Gen_hair(int amount, int segments, GLfloat length);

        void Draw_floor();
        void Draw_hair();

    private:

        const static GLfloat floorVerts[];
        const static GLubyte floorInds[];

        GLuint floorVAid, floorVBid;
        GLuint hairPosid, hairVBid;

        std::vector<GLfloat> hairVerts;
        std::vector<glm::vec2> hairPos;

};
