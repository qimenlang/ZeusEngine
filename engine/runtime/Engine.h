#pragma once

#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include <Singleton.h>

namespace Zeus {
// settings
const static unsigned int SCR_WIDTH = 1920;
const static unsigned int SCR_HEIGHT = 1080;
static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

static glm::mat4 projection = glm::perspective(
    glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
static Camera camera(cameraPos, cameraUp);

class Engine : public Singleton<Engine> {
  friend class Singleton<Engine>; // 允许访问私有成员函数
private:
  float m_deltaTime = 0.0f;   // 当前帧与上一帧的时间差
  float m_currentTime = 0.0f; // 当前帧时间
  float m_lastFrame = 0.0f;   // 上一帧的时间
  /* data */
public:
  Engine(/* args */);
  ~Engine();
  void init();
  void update();

  float deltaTime() const { return m_deltaTime; }
  float currentTime() const { return m_currentTime; }
};

} // namespace Zeus