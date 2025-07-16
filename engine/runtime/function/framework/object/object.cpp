#include "function/framework/object/object.h"

#include <memory>

#include "Engine.h"
#include "resource/asset_manager.h"

void Object::tick() {
    if (m_shader.expired()) {
        std::cerr << "Object shader is expired!" << std::endl;
        return;
    }

    if (onTick) onTick.invoke(this);

    m_shader.lock()->use();
    m_shader.lock()->setMat4("model", m_transform->GetModelMatrix());
    m_shader.lock()->setMat4(
        "view", Zeus::Engine::getInstance().camera().GetViewMatrix());
    m_shader.lock()->setMat4("projection", Zeus::projection);

    m_transform->tick(0);
    m_mesh_component->tick(0);
}
