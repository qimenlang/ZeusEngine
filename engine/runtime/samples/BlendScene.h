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

    std::map<float, std::unique_ptr<Object>> m_ts_obj_map;

   public:
    BlendScene(/* args */) {};
    ~BlendScene() {};
    void init() override;
    void update() override;
};
