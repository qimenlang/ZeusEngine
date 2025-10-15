#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"

class TransformComponent : public Component {
   public:
    TransformComponent(/* args */) {};
    ~TransformComponent() {};

    void tick(float delta_time) override;

    void setPosition(glm::vec3 position) { m_position = position; }
    glm::vec3 position() const { return m_position; }
    void setRotation(glm::vec3 rotateAxis, float rotation);
    void setScale(glm::vec3 scale) { m_scale = scale; }

    glm::mat4 GetModelMatrix();

   private:
    glm::vec3 m_position = glm::vec3{0.f};
    glm::vec3 m_rotateAxis = glm::vec3{1.f, 0, 0};
    float m_rotation = 0.f;
    glm::vec3 m_scale = glm::vec3{1.f};
};
