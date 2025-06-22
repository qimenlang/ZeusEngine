#include "Scene.h"
#include "Shader.h"

Scene::Scene() {
  // 初始化场景
}
Scene::~Scene() {
  // 清理场景
  m_models.clear();
}
void Scene::update() {
  // 更新场景中的所有模型
  for (auto &model : m_models) {
    if (model) {
      model->Draw();
    }
  }
}