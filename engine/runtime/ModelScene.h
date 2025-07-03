#pragma once
#include "Model.h"
#include "Scene.h"
#include "Shader.h"

class ModelScene : public Scene {
private:
  /* data */
  std::shared_ptr<Shader> m_lightShader;
  std::unique_ptr<Model> m_lightCube;

  std::shared_ptr<Shader> m_phongShader;
  std::unique_ptr<Model> m_dragon;

public:
  ModelScene(/* args */);
  ~ModelScene();
  void init() override;
  void update() override;
};
