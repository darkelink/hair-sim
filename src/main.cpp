/*
 * particles - Simple GPU particle hair simulation
 * 2015 - John Gresty
 *
 * See LICENSE file for copyright and license details.
 *
 */


#include "scene.h"
#include "shader.h"
#include "camera.h"
#include "controller.h"
#include "params.h"

#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


Params
init_params() {
    Params p;

    // hair
    p.Add("strands", 10000.0f);
    p.Add("segments", 5.0f);
    p.Add("length", 0.5f);

    // wind
    p.Add("direction", 2.1f);
    p.Add("speed", 1.0f);
    p.Add("strength", 1.0f);

    return p;
}


int
main(int argc, char* argv[]) {
    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW" << std::endl;
        return 1;
    }

    int winWidth = 1280;
    int winHeight = 720;

    int testHeight, testWidth;

    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Particles", NULL, NULL);
    if (!window) {
        std::cerr << "ERROR: could not open window" << std::endl;
        glfwTerminate();
        return 2;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) {
        std::cerr << "ERROR: could not start GLEW" << std::endl;
        return 3;
    }

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version  = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version: " << version << std::endl;

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwPollEvents();

    Params params = init_params();

    Scene scene;
    scene.Load();
    scene.Gen_hair(
        (int)params.Get_value("strands"),
        (int)params.Get_value("segments"),
        params.Get_value("length"));


    Shader floorShader;
    floorShader.Load(std::ifstream("res/shaders/basic.vert.glsl"), GL_VERTEX_SHADER);
    floorShader.Load(std::ifstream("res/shaders/basic.frag.glsl"), GL_FRAGMENT_SHADER);
    floorShader.Link();

    Shader hairShader;
    hairShader.Load(std::ifstream("res/shaders/hair.vert.glsl"), GL_VERTEX_SHADER);
    hairShader.Load(std::ifstream("res/shaders/hair.frag.glsl"), GL_FRAGMENT_SHADER);
    hairShader.Link();

    Camera camera;
    camera.Setup();

    Controller controller(window, &params, &scene);
    controller.Set_camera(&camera);

    double currentTime, lastTime = 0;
    float deltaTime;

    // grr, callbacks
    glfwSetScrollCallback(window, Controller::Zoom);
    glfwSetCharCallback(window, Controller::Text);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentTime = glfwGetTime();
        deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;
        controller.Get_input();
        controller.Update_view(deltaTime);

        floorShader.Use();
        floorShader.Set_uniformm4fv("MVP", camera.mvp);
        scene.Draw_floor();

        hairShader.Use();
        hairShader.Set_uniformm4fv("MVP", camera.mvp);
        hairShader.Set_uniform1f("direction", params.Get_value("direction"));
        hairShader.Set_uniform1f("time", currentTime * params.Get_value("speed"));
        hairShader.Set_uniform1f("strength", params.Get_value("strength"));
        scene.Draw_hair();

        glfwSwapBuffers(window);
        glfwPollEvents();

        glfwGetWindowSize(window, &testWidth, &testHeight);
        if (testWidth != winWidth || testHeight != winHeight) {
            winWidth = testWidth;
            winHeight = testHeight;
            camera.Set_window_size(winWidth, winHeight);
            glViewport(0, 0, winWidth, winHeight);
        }

    } while(!glfwWindowShouldClose(window));

    glfwTerminate();

    return 0;
}
