#pragma once
#include "Scene.h"
#include "function/render/Shader.h"

class Object;

class CubeScene : public Scene {
   private:
    /* data */
    std::unique_ptr<Object> m_lightCube;

    std::unique_ptr<Object> m_cube;
    std::unique_ptr<Object> m_cube1;
    std::unique_ptr<Object> m_cube2;
    std::unique_ptr<Object> m_cube3;
    std::unique_ptr<Object> m_cube4;

   public:
    CubeScene(/* args */);
    ~CubeScene();
    void init() override;
    void update() override;
};
