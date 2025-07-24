#pragma once
#include "Scene.h"

class Object;

class DepthScene : public Scene {
   private:
    /* data */
    std::unique_ptr<Object> m_cube1;
    std::unique_ptr<Object> m_cube2;
    std::unique_ptr<Object> m_cube3;
    std::unique_ptr<Object> m_floor;

   public:
    DepthScene(/* args */) {};
    ~DepthScene() {};
    void init() override;
    void update() override;
};
