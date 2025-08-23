#pragma once
#include "function/framework/scene/Scene.h"

class Object;

class FBOScene : public Scene {
   private:
    /* data */
    std::vector<std::unique_ptr<Object>> m_objects;

    std::unique_ptr<Object> m_quad;

    unsigned int fbo;

   public:
    FBOScene(/* args */);
    ~FBOScene();
    void init() override;
    void update() override;
};
