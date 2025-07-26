#pragma once

#include <Singleton.h>

#include <memory>

#include "Camera.h"
#include "resource/AssetManager.h"

namespace Zeus {
// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
inline glm::mat4 projection = glm::perspective(
    glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

class Engine : public Singleton<Engine> {
    friend class Singleton<Engine>;  // 允许访问私有成员函数
   private:
    float m_deltaTime = 0.0f;    // 当前帧与上一帧的时间差
    float m_currentTime = 0.0f;  // 当前帧时间
    float m_lastFrame = 0.0f;    // 上一帧的时间
    /* data */

    Camera m_camera;
    std::unique_ptr<AssetManager> m_assetManager;

   public:
    Engine(/* args */);
    ~Engine();
    void init();
    void update();

    float deltaTime() const { return m_deltaTime; }
    float currentTime() const { return m_currentTime; }

    Camera& camera() { return m_camera; }
    AssetManager& assetManager() { return *m_assetManager; }
};

}  // namespace Zeus