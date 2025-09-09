#pragma once
#include "function/framework/scene/Scene.h"

class Object;

class ShadowScene : public Scene {
   private:
    /* data */
    std::vector<std::unique_ptr<Object>> m_objects;
    std::unique_ptr<Object> m_lightCube;

   public:
    ShadowScene(/* args */);
    ~ShadowScene();
    void init() override;
    void update() override;
};
