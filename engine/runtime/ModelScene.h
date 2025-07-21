#pragma once
#include "Scene.h"
#include "function/framework/object/object.h"
#include "function/render/Shader.h"

class ModelScene : public Scene {
   private:
    /* data */
    std::shared_ptr<Shader> m_lightShader;
    std::unique_ptr<Object> m_lightCube;

    std::shared_ptr<Shader> m_phongShader;
    std::unique_ptr<Object> m_dragon;

   public:
    ModelScene(/* args */);
    ~ModelScene();
    void init() override;
    void update() override;
};
