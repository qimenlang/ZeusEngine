#pragma once
#include <map>

#include "Scene.h"

class Object;

class BlendScene : public Scene {
   private:
    /* data */
    std::unique_ptr<Object> m_cube1;
    std::unique_ptr<Object> m_hand;
    std::unique_ptr<Object> m_floor;
    std::vector<std::unique_ptr<Object>> m_ts_objs;

   public:
    BlendScene(/* args */) {};
    ~BlendScene() {};
    void init() override;
    void update() override;
};
