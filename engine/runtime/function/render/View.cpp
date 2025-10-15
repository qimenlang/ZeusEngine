#include "View.h"

Scene* View::getScene() {
    if (m_scene) {
        return m_scene.get();
    }
    return nullptr;
}

void View::update() { m_scene->update(); }
