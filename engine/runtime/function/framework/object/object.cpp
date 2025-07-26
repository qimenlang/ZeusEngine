#include "function/framework/object/Object.h"

#include <memory>

#include "Engine.h"
#include "Object.h"
#include "resource/AssetManager.h"

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

    if (!m_material) {
        std::cerr << "Object material is expired!" << std::endl;
        return;
    }
    m_material->use();
    m_material->shader()->setMat4("model", m_transform->GetModelMatrix());
    m_material->shader()->setMat4(
        "view", Zeus::Engine::getInstance().camera().GetViewMatrix());
    m_material->shader()->setMat4("projection", Zeus::projection);

    for (auto& component : m_components) {
        component->tick(0);
    }
}
