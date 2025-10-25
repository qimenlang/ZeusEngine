
#pragma once
#include "Geometry.h"

void Geometry::updateBoundingBox() {
    if (m_vertices.size() == 0) {
        m_boundingBox.min = glm::vec3(0.0f);
        m_boundingBox.max = glm::vec3(0.0f);
        return;
    }

    glm::vec3 min = glm::vec3(m_vertices[0].Position);
    glm::vec3 max = glm::vec3(m_vertices[0].Position);

    for (const auto& vertex : m_vertices) {
        glm::vec3 pos = glm::vec3(vertex.Position);
        min = glm::min(min, pos);
        max = glm::max(max, pos);
    }

    m_boundingBox.min = min;
    m_boundingBox.max = max;
}
