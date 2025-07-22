#include "function/framework/object/object.h"

#include <memory>

#include "Engine.h"
#include "object.h"
#include "resource/asset_manager.h"

Component* Object::addComponent(std::unique_ptr<Component>&& component) {
    auto ptr = component.get();
    m_components.emplace_back(std::move(component));
    ptr->doAdd(this);
    return ptr;
}
void Object::removeComponent(Component* componentIn) {
    auto itr = m_components.begin();
    while (itr != m_components.end()) {
        if (itr->get() == componentIn) break;
        itr++;
    }
    m_components.erase(itr);
};

void Object::tick() {
    if (onTick) onTick.invoke(this);
    m_transform->tick(0);

    if (m_shader.expired()) {
        std::cerr << "Object shader is expired!" << std::endl;
        return;
    }
    m_shader.lock()->use();
    m_shader.lock()->setMat4("model", m_transform->GetModelMatrix());
    m_shader.lock()->setMat4(
        "view", Zeus::Engine::getInstance().camera().GetViewMatrix());
    m_shader.lock()->setMat4("projection", Zeus::projection);

    m_mesh_component->tick(0);
}
