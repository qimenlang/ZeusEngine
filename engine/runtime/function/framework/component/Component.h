#pragma once

#include <memory>
class Object;

class Component {
   public:
    Component() = default;
    virtual ~Component() {};

    // Instantiating the component after definition loaded
    virtual void postLoadResource(std::weak_ptr<Object> parent_object) {
        m_parent_object = parent_object;
    }

    virtual void tick(float delta_time) = 0;

   private:
    std::weak_ptr<Object> m_parent_object;
};
