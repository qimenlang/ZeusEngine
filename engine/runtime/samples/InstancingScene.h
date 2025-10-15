#pragma once
#include "function/framework/scene/Scene.h"
#include "function/render/Shader.h"

class InstancingScene : public Scene {
   public:
    InstancingScene(/* args */) {};
    ~InstancingScene() {};
    void init() override;
    void update() override;

   private:
    Shader m_shader;
    unsigned int instanceVBO;
    unsigned int vao, vbo;
};
