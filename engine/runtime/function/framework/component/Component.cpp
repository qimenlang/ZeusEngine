#include "Component.h"

#include "function/framework/object/Object.h"

void Component::addToObject(Object& obj,
                            std::unique_ptr<Component>&& component) {
    obj.addComponent(std::move(component));
}