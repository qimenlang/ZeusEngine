#pragma once

#include <memory>
class Object;

class Component {
public:
  Component() = default;
  virtual ~Component() {};

  virtual void tick(float delta_time) = 0;

private:
  std::weak_ptr<Object> m_parent_bject;
};
