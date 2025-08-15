#pragma once
#include "Scene.h"

class Object;

class CubeFBOScene : public Scene {
   private:
    /* data */
    std::vector<std::unique_ptr<Object>> m_objects;

    std::unique_ptr<Object> m_quad;

   public:
    CubeFBOScene(/* args */);
    ~CubeFBOScene();
    void init() override;
    void update() override;
};
