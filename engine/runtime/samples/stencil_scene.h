#pragma once
#include "Scene.h"

class Object;

class StencilScene : public Scene {
   private:
    /* data */
    std::unique_ptr<Object> m_cube1;
    std::unique_ptr<Object> m_cube2;
    std::unique_ptr<Object> m_floor;

   public:
    StencilScene(/* args */) {};
    ~StencilScene() {};
    void init() override;
    void update() override;
};
