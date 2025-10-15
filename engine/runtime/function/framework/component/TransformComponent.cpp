#include "TransformComponent.h"

void TransformComponent::tick(float delta_time) {}

void TransformComponent::setRotation(glm::vec3 rotateAxis, float rotation) {
    m_rotateAxis = rotateAxis;
    m_rotation = rotation;
}

glm::mat4 TransformComponent::GetModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    model = glm::rotate(model, m_rotation, m_rotateAxis);
    model = glm::scale(model, m_scale);
    return model;
}
