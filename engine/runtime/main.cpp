#include "include/glad/glad.h"
// glad.h must before glfw3.h
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "Engine.h"
#include "function/framework/object/Object.h"
#include "function/render/Renderer.h"
#include "function/render/View.h"
#include "samples/BlendScene.h"
#include "samples/CubeScene.h"
#include "samples/DepthScene.h"
#include "samples/FBOScene.h"
#include "samples/InstancingScene.h"
#include "samples/InstancingStarScene.h"
#include "samples/ModelScene.h"
#include "samples/PBRScene.h"
#include "samples/PBRTextureScene.h"
#include "samples/StencilScene.h"

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
        glfwTerminate();
        return -1;
    }
    const char *versionStr = (const char *)glGetString(GL_VERSION);
    std::cout << "OpenGL版本: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "显卡供应商: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "渲染器: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GLSL版本: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
              << std::endl;

    // 捕捉光标，并隐藏，光标不显示，且不会离开窗口
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#ifdef ZEUS_ROOT_DIR
    PRINTAPI(ZEUS_ROOT_DIR);
    std::cout << ZEUS_ROOT_DIR << std::endl;
#endif

    Zeus::Engine::getInstance().camera().MouseSensitivity = 0.01f;

    // auto sczene = std::make_unique<CubeScene>();
    // auto sczene = std::make_unique<FBOScene>();
    // auto scene = std::make_unique<ModelScene>();
    // auto scene = std::make_unique<DepthScene>();
    // auto scene = std::make_unique<StencilScene>();
    // auto scene = std::make_unique<BlendScene>();
    // auto scene = std::make_unique<PBRScene>();
    // auto scene = std::make_unique<PBRTextureScene>();
    // auto scene = std::make_unique<InstancingScene>();
    auto scene = std::make_unique<InstancingStarScene>();

    scene->init();
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
    std::unique_ptr<View> view = std::make_unique<View>();
    view->setScene(std::move(scene));

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // logic
        engine.update();
        // inputd
        processInput(window);

        renderer->prerender();
        renderer->render(view.get());

        // check poll events & swap buffer
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}