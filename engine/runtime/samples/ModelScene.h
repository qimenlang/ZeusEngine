#pragma once
#include "function/framework/object/Object.h"
#include "function/framework/scene/Scene.h"


class ModelScene : public Scene {
   private:
    /* data */
    std::unique_ptr<Object> m_lightCube;

    std::unique_ptr<Object> m_dragon;

   public:
    ModelScene(/* args */);
    ~ModelScene();
    void init() override;
    void update() override;
};
