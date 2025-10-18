#include "include/glad/glad.h"
// glad.h must before glfw3.h
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Engine.h"
#include "function/framework/object/Object.h"
#include "function/render/Renderer.h"
#include "function/render/View.h"
#include "samples/BlendScene.h"
#include "samples/ComputeShaderScene.h"
#include "samples/CubeScene.h"
#include "samples/DepthScene.h"
#include "samples/FBOScene.h"
#include "samples/InstancingScene.h"
#include "samples/InstancingStarScene.h"
#include "samples/ModelScene.h"
#include "samples/PBRScene.h"
#include "samples/PBRTextureScene.h"
#include "samples/ShadowScene.h"
#include "samples/StencilScene.h"

using namespace std;

#define PRINTAPI(x) std::cout << #x << std::endl;

// float engine.deltaTime() = 0.0f; // 当前帧与上一帧的时间差
// float lastFrame = 0.0f; // 上一帧的时间

float lastX = Zeus::SCR_WIDTH / 2.0f;
float lastY = Zeus::SCR_HEIGHT / 2.0f;

auto &engine = Zeus::Engine::getInstance();

static int savedWidth, savedHeight, savedX, savedY;
static bool isFullscreen = false;
void toggleFullscreen(GLFWwindow *window) {
    std::cout << "toggleFullscreen : " << isFullscreen << std::endl;
    if (!isFullscreen) {
        // 获取显示器信息
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        // 切换到全屏
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height,
                             mode->refreshRate);
        isFullscreen = true;
    } else {
        // 切换回窗口模式
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        glfwSetWindowMonitor(window, NULL, savedX, savedY, savedWidth,
                             savedHeight, 0);
        isFullscreen = false;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    std::cout << "framebuffer_size_callback [" << width << "," << height << "]"
              << std::endl;
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    std::cout << "mouse(" << xpos << " , " << ypos << " )" << std::endl;

    float xoffset = xpos - lastX;
    float yoffset =
        lastY - ypos;  // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (!Zeus::Engine::getInstance().camera().isRotationMode) return;
    Zeus::Engine::getInstance().camera().ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            Zeus::Engine::getInstance().camera().isRotationMode = true;
        }
        if (action == GLFW_RELEASE) {
            Zeus::Engine::getInstance().camera().isRotationMode = false;
        }
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    Zeus::Engine::getInstance().camera().ProcessMouseScroll(yoffset);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        toggleFullscreen(window);
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// 持续事件在轮训中处理
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Zeus::Engine::getInstance().camera().ProcessMovement(
            Camera_Movement::FORWARD, engine.deltaTime());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Zeus::Engine::getInstance().camera().ProcessMovement(
            Camera_Movement::BACKWARD, engine.deltaTime());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Zeus::Engine::getInstance().camera().ProcessMovement(
            Camera_Movement::LEFT, engine.deltaTime());
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Zeus::Engine::getInstance().camera().ProcessMovement(
            Camera_Movement::RIGHT, engine.deltaTime());
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        Zeus::Engine::getInstance().camera().ProcessMovement(
            Camera_Movement::UP, engine.deltaTime());
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        Zeus::Engine::getInstance().camera().ProcessMovement(
            Camera_Movement::DOWN, engine.deltaTime());
    // if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_) {
    //     toggleFullscreen(window);
    // }
}

int main() {
    // init glfw
    std::cout << "Zeus Engine Start" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(
        glfwGetPrimaryMonitor());  // Valid on GLFW 3.3+ only

    GLFWwindow *window = glfwCreateWindow((int)(Zeus::SCR_WIDTH * main_scale),
                                          (int)(Zeus::SCR_HEIGHT * main_scale),
                                          "Zeus", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwGetWindowPos(window, &savedX, &savedY);
    glfwGetWindowSize(window, &savedWidth, &savedHeight);
    std::cout << "window Pos:{" << savedX << ", " << savedY
              << "}; w:" << savedWidth << ",h:" << savedHeight << std::endl;

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // 瞬时事件在回调中处理
    glfwSetKeyCallback(window, key_callback);
    //  glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    const char *versionStr = (const char *)glGetString(GL_VERSION);
    const char *glslVersionStr =
        (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "OpenGL版本: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "显卡供应商: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "渲染器: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GLSL版本: " << glslVersionStr << std::endl;

    // 捕捉光标，并隐藏，光标不显示，且不会离开窗口
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

#ifdef ZEUS_ROOT_DIR
    PRINTAPI(ZEUS_ROOT_DIR);
    std::cout << ZEUS_ROOT_DIR << std::endl;
#endif

    // auto scene = std::make_unique<CubeScene>();
    // auto scene = std::make_unique<ModelScene>();
    // auto scene = std::make_unique<DepthScene>();
    // auto scene = std::make_unique<StencilScene>();
    // auto scene = std::make_unique<BlendScene>();
    auto scene = std::make_unique<PBRScene>();
    // auto scene = std::make_unique<PBRTextureScene>();
    // auto scene = std::make_unique<FBOScene>();
    // auto scene = std::make_unique<InstancingScene>();
    // auto scene = std::make_unique<InstancingStarScene>();
    // auto scene = std::make_unique<ComputerShaderScene>();
    // auto scene = std::make_unique<ShadowScene>();

    scene->init();
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
    std::unique_ptr<View> view = std::make_unique<View>();
    view->setScene(std::move(scene));

    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(
        window, true);  // Second param install_callback=true will install
                        // GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 460");

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // poll events
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // logic
        engine.update();
        auto title = "FPS:" + std::to_string(engine.fps());
        glfwSetWindowTitle(window, title.c_str());

        // input
        processInput(window);

        renderer->prerender();
        renderer->render(view.get());

        // Gui Rendering
        // (Your code clears your framebuffer, renders your other stuff etc.)
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // check swap buffer
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}