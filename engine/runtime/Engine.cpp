#include "Engine.h"

#include <GLFW/glfw3.h>

namespace Zeus {
Engine::Engine(/* args */) { init(); }

Engine::~Engine() {}

void Engine::init() {
    // 初始化引擎
    std::cout << "Engine initialized." << std::endl;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    m_camera = Camera(cameraPos);
    m_assetManager = std::make_unique<AssetManager>();
}

void Engine::update() {
    // 更新引擎状态
    m_currentTime = glfwGetTime();
    m_deltaTime = m_currentTime - m_lastFrame;
    m_lastFrame = m_currentTime;
    // 更新帧率信息
    if (m_currentTime - m_lastFPSUpdateTime > 1.0f) {
        m_fps = 1.0 / m_deltaTime;
        m_lastFPSUpdateTime = m_currentTime;
    }

    // std::cout << "Engine updated. Delta Time: " << m_deltaTime << std::endl;
    // std::cout << "Engine updated. FPS: " << 1.0 / m_deltaTime << std::endl;
}
}  // namespace Zeus