#pragma once
#include <memory>
#include <vector>

#include "function/framework/object/object.h"

using namespace std;

// 管理场景
class Scene {
   private:
    vector<unique_ptr<Object>> m_models;

   protected:
    void addModel(unique_ptr<Object> model) { m_models.push_back(move(model)); }

   public:
    Scene(/* args */);
    virtual ~Scene();
    virtual void init() = 0;
    virtual void update();
};
