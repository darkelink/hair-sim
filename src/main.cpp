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

    p.Add("fov", 45.0f);

    p.Add("floor_red", 0.1f);
    p.Add("floor_green", 0.04f);
    p.Add("floor_blue", 0.02f);
    p.Add("floor_alpha", 1.0f);

    // hair
    p.Add("strands", 1000.0f);
    p.Add("segments", 5.0f);
    p.Add("length", 0.5f);
    p.Add("width", 1.0f);

    p.Add("hair_red", 0.3f);
    p.Add("hair_green", 0.1f);
    p.Add("hair_blue", 0.05f);
    p.Add("hair_alpha", 0.3f);

    // wind
    p.Add("direction", 0.0f);
    p.Add("speed", 1.0f);
    p.Add("strength", 1.0f);
    p.Add("frequency", 1.0f);
    p.Add("iterations", 5.0f);

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
    floorShader.Load(std::ifstream("res/shaders/passthrough.frag.glsl"), GL_FRAGMENT_SHADER);
    floorShader.Link();

    Shader hairShader;
    hairShader.Load(std::ifstream("res/shaders/hair.vert.glsl"), GL_VERTEX_SHADER);
    hairShader.Load(std::ifstream("res/shaders/passthrough.frag.glsl"), GL_FRAGMENT_SHADER);
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
        floorShader.Set_uniform4f("mycolor", glm::vec4(
            params.Get_value("floor_red"),
            params.Get_value("floor_green"),
            params.Get_value("floor_blue"),
            params.Get_value("floor_alpha")));
        scene.Draw_floor();

        hairShader.Use();
        hairShader.Set_uniformm4fv("MVP", camera.mvp);
        hairShader.Set_uniform1f("direction", params.Get_value("direction"));
        hairShader.Set_uniform1f("time", currentTime * params.Get_value("speed"));
        hairShader.Set_uniform1f("strength", params.Get_value("strength"));
        hairShader.Set_uniform1f("frequency", params.Get_value("frequency"));
        hairShader.Set_uniform1i("iterations", (int)params.Get_value("iterations"));

        hairShader.Set_uniform4f("mycolor", glm::vec4(
            params.Get_value("hair_red"),
            params.Get_value("hair_green"),
            params.Get_value("hair_blue"),
            params.Get_value("hair_alpha")));
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
