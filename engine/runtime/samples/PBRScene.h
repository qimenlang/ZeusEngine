#pragma once
#include "Scene.h"

class Object;

class PBRScene : public Scene {
   private:
    /* data */
    std::vector<std::unique_ptr<Object>> m_lights;
    std::vector<std::unique_ptr<Object>> m_spheres;

   public:
    PBRScene(/* args */);
    ~PBRScene();
    void init() override;
    void update() override;
};
