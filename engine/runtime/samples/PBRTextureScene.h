#pragma once
#include "Scene.h"

class Object;

class PBRTextureScene : public Scene {
   private:
    /* data */
    std::vector<std::unique_ptr<Object>> m_lights;
    std::vector<std::unique_ptr<Object>> m_spheres;

   public:
    PBRTextureScene(/* args */);
    ~PBRTextureScene();
    void init() override;
    void update() override;
};
