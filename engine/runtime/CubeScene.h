#pragma once
#include "Model.h"
#include "Scene.h"
#include "Shader.h"

class CubeScene : public Scene {
   private:
    /* data */
    std::shared_ptr<Shader> m_lightShader;
    std::unique_ptr<Model> m_lightCube;

    std::shared_ptr<Shader> m_phongSampleShader;
    std::unique_ptr<Model> m_cube;
    std::unique_ptr<Model> m_cube1;
    std::unique_ptr<Model> m_cube2;
    std::unique_ptr<Model> m_cube3;
    std::unique_ptr<Model> m_cube4;

   public:
    CubeScene(/* args */);
    ~CubeScene();
    void init() override;
    void update() override;
};
