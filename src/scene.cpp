#include "scene.h"

#include <iostream>

const GLfloat Scene::floorVerts[] = {
    -1.0f, 0.0f, -1.0f,
    -1.0f, 0.0f,  1.0f,
     1.0f, 0.0f,  1.0f,
     1.0f, 0.0f, -1.0f
};
const GLubyte Scene::floorInds[] = {0,1,2,0,2,3};


Scene::Scene() {}
Scene::~Scene() {}


bool Scene::Load() {
    glGenVertexArrays(1, &floorVAid);
    glBindVertexArray(floorVAid);

    glGenBuffers(1, &floorVBid);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVerts), floorVerts, GL_STATIC_DRAW);

    return true;
}

void Scene::Gen_hair(int amount, int segments, GLfloat length) {

    hairVerts.clear();
    hairVerts.reserve(segments);
    for (int i = 0; i < segments; ++i) {
        hairVerts.push_back(((float)i/(float)segments) * length);
    }

    glGenBuffers(1, &hairVBid);
    glBindBuffer(GL_ARRAY_BUFFER, hairVBid);
    glBufferData(GL_ARRAY_BUFFER, hairVerts.size() * sizeof(GLfloat), &hairVerts.front(), GL_STATIC_DRAW);


    hairPos.clear();
    hairPos.reserve(amount);
    for (int i = 0; i < amount; ++i) {
        hairPos.push_back(glm::vec2(
                    static_cast<float>(rand())/ static_cast<float>(RAND_MAX/2.0f) - 1.0f,
                    static_cast<float>(rand())/ static_cast<float>(RAND_MAX/2.0f) - 1.0f));
    }

    glGenBuffers(1, &hairPosid);
    glBindBuffer(GL_ARRAY_BUFFER, hairPosid);
    glBufferData(GL_ARRAY_BUFFER, hairPos.size() * sizeof(glm::vec2), &hairPos.front(), GL_STATIC_DRAW);
}

void Scene::Draw_floor() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBid);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawElements(GL_TRIANGLES, sizeof(floorInds), GL_UNSIGNED_BYTE, floorInds);

    glDisableVertexAttribArray(0);
}

void Scene::Draw_hair() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, hairVBid);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, hairPosid);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribDivisor(1, 1);

    glDrawArraysInstanced(GL_LINE_STRIP, 0, hairVerts.size(), hairPos.size());
}
