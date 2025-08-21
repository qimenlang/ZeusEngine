#pragma once
#include "Scene.h"

class Object;

class CubeFBOScene : public Scene {
   private:
    /* data */
    std::vector<std::unique_ptr<Object>> m_objects;

    std::unique_ptr<Object> m_quad;

    unsigned int fbo;
    unsigned int textureColorbuffer;

    Shader m_screenShader;
    unsigned int quadVAO, quadVBO;

   public:
    CubeFBOScene(/* args */);
    ~CubeFBOScene();
    void init() override;
    void update() override;
};
