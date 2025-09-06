#pragma once

#include <function/framework/scene/Scene.h>
#include <function/render/ComputeShader.h>

#include <map>

class Object;

class ComputerShaderScene : public Scene {
   private:
    /* data */
    std::unique_ptr<Object> m_quad;

    std::unique_ptr<ComputeShader> m_computeShader;

    int m_counter = 0;

   public:
    ComputerShaderScene(/* args */) {};
    ~ComputerShaderScene() {};
    void init() override;
    void update() override;
};
