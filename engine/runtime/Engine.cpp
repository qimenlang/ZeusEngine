#include "Engine.h"

#include <GLFW/glfw3.h>

namespace Zeus {
Engine::Engine(/* args */) { init(); }

Engine::~Engine() {}

void Engine::init() {
    // 初始化引擎
    std::cout << "Engine initialized." << std::endl;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    m_camera = Camera(cameraPos, cameraUp);
}

void Engine::update() {
    // 更新引擎状态
    m_currentTime = glfwGetTime();
    m_deltaTime = m_currentTime - m_lastFrame;
    m_lastFrame = m_currentTime;
    // std::cout << "Engine updated. Delta Time: " << deltaTime << std::endl;
}
}  // namespace Zeus