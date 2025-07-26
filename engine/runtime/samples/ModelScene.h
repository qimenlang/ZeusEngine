#pragma once
#include "Scene.h"
#include "function/framework/object/Object.h"

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
