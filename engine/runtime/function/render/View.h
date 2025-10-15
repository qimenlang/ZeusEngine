#pragma once
#include <memory>
#include <string>

#include "function/framework/scene/Scene.h"

class View {
   public:
    View(/* args */) {};
    ~View() {};
    void setScene(std::unique_ptr<Scene> scene) { m_scene = std::move(scene); }
    Scene *getScene();

    void update();

   private:
    std::string m_name;
    std::unique_ptr<Scene> m_scene;
};
