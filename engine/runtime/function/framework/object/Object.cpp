#include "Object.h"

#include "framework/component/Component.h"

Object::Object(/* args */) {}

Object::~Object() {}

void Object::tick(float delta_time) {
    for (auto &component : m_components) {
        component.tick(delta_time);
    }
}

bool Object::load(const std::string &path) {}
