#include "Engine.h"
#include <GLFW/glfw3.h>

namespace Zeus {
Engine::Engine(/* args */) {}

Engine::~Engine() {}

void Engine::init() {
  // 初始化引擎
  std::cout << "Engine initialized." << std::endl;
}

void Engine::update() {
  // 更新引擎状态
  m_currentTime = glfwGetTime();
  m_deltaTime = m_currentTime - m_lastFrame;
  m_lastFrame = m_currentTime;
  // std::cout << "Engine updated. Delta Time: " << deltaTime << std::endl;
}
} // namespace Zeus