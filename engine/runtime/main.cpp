#include "include/glad/glad.h"
// glad.h must before glfw3.h
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "CubeScene.h"
#include "Engine.h"
#include "ModelScene.h"
#include "function/framework/object/object.h"
#include "function/render/Shader.h"

using namespace std;

#define PRINTAPI(x) std::cout << #x << std::endl;

// float engine.deltaTime() = 0.0f; // 当前帧与上一帧的时间差
// float lastFrame = 0.0f; // 上一帧的时间

float lastX = Zeus::SCR_WIDTH / 2.0f;
float lastY = Zeus::SCR_HEIGHT / 2.0f;
bool firstMouse = true;

auto &engine = Zeus::Engine::getInstance();

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    std::cout << "framebuffer_size_callback [" << width << "," << height << "]"
              << std::endl;
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    std::cout << "mouse(" << xpos << " , " << ypos << " )" << std::endl;
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset =
        lastY - ypos;  // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    Zeus::Engine::getInstance().camera().ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    Zeus::Engine::getInstance().camera().ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::cout << "processInput " << GLFW_KEY_ESCAPE << std::endl;
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        std::cout << "deltaTime: " << engine.deltaTime() << std::endl;
        Zeus::Engine::getInstance().camera().ProcessKeyboard(
            Camera_Movement::FORWARD, engine.deltaTime());
        std::cout << "Camera Position: "
                  << glm::to_string(
                         Zeus::Engine::getInstance().camera().Position)
                  << " ,camera :" << &Zeus::Engine::getInstance().camera()
                  << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Zeus::Engine::getInstance().camera().ProcessKeyboard(
            Camera_Movement::BACKWARD, engine.deltaTime());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Zeus::Engine::getInstance().camera().ProcessKeyboard(
            Camera_Movement::LEFT, engine.deltaTime());
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Zeus::Engine::getInstance().camera().ProcessKeyboard(
            Camera_Movement::RIGHT, engine.deltaTime());
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        Zeus::Engine::getInstance().camera().ProcessKeyboard(
            Camera_Movement::UP, engine.deltaTime());
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        Zeus::Engine::getInstance().camera().ProcessKeyboard(
            Camera_Movement::DOWN, engine.deltaTime());
}

int main() {
    // init glfw
    std::cout << "Zeus Engine Start" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    GLFWwindow *window = glfwCreateWindow(Zeus::SCR_WIDTH, Zeus::SCR_HEIGHT,
                                          "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //  glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // 捕捉光标，并隐藏，光标不显示，且不会离开窗口
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#ifdef ZEUS_ROOT_DIR
    PRINTAPI(ZEUS_ROOT_DIR);
    std::cout << ZEUS_ROOT_DIR << std::endl;
#endif

    Zeus::Engine::getInstance().camera().MouseSensitivity = 0.01f;

    // CubeScene cubeScene;
    // cubeScene.init();
    ModelScene modelScene;
    modelScene.init();

    glEnable(GL_DEPTH_TEST);
    // render loop
    while (!glfwWindowShouldClose(window)) {
        // logic
        engine.update();
        // inputd
        processInput(window);
        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // cubeScene.update();
        modelScene.update();

        // check poll events & swap buffer
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}