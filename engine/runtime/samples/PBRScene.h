#pragma once
#include "function/framework/scene/Scene.h"

class Object;
/*
    PBR 效果测试
    SSS 效果测试
*/
class PBRScene : public Scene {
   private:
    /* data */
    std::vector<std::unique_ptr<Object>> m_lights;
    std::vector<std::unique_ptr<Object>> m_spheres;
    std::vector<std::unique_ptr<Object>> m_models;

   public:
    PBRScene(/* args */);
    ~PBRScene();
    void init() override;
    void update() override;
};
