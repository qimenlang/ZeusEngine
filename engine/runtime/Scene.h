#pragma once
#include <memory>
#include <vector>

#include "Model.h"

using namespace std;

// 管理场景
class Scene {
   private:
    vector<unique_ptr<Model>> m_models;

   protected:
    void addModel(unique_ptr<Model> model) { m_models.push_back(move(model)); }

   public:
    Scene(/* args */);
    virtual ~Scene();
    virtual void init() = 0;
    virtual void update();
};
