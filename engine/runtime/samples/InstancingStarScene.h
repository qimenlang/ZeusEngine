#pragma once
#include "function/framework/object/Object.h"
#include "function/framework/scene/Scene.h"
#include "function/render/Shader.h"

class InstancingStarScene : public Scene {
   public:
    InstancingStarScene(/* args */) {};
    ~InstancingStarScene() {};
    void init() override;
    void update() override;

   private:
    std::vector<std::unique_ptr<Object>> m_objects;
};
