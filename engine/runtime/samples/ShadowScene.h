#pragma once
#include "function/framework/scene/Scene.h"

class Object;

class ShadowScene : public Scene {
   private:
    /* data */
    std::vector<std::unique_ptr<Object>> m_objects;
    std::unique_ptr<Object> m_lightCube;

    std::vector<std::unique_ptr<Object>> m_shadowObjects;

    unsigned int fbo;

    std::unique_ptr<Object> m_quad;

    glm::mat4 lightSpaceMatrix;

   public:
    ShadowScene(/* args */);
    ~ShadowScene();
    void init() override;
    void update() override;
};
