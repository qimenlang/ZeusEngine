#pragma once
#include "function/framework/scene/Scene.h"

class Object;

class CubeFBOScene : public Scene {
   private:
    /* data */
    std::vector<std::unique_ptr<Object>> m_objects;

    std::unique_ptr<Object> m_quad;

    unsigned int fbo;

   public:
    CubeFBOScene(/* args */);
    ~CubeFBOScene();
    void init() override;
    void update() override;
};
