#include "SphereGeometry.h"

#include <numbers>

Geometry SphereGeometry::create(float radius, GeometryDescriptor desc) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359f;
    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos =
                std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos =
                std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            Vertex vert;
            vert.Position =
                glm::vec4(glm::vec3{xPos, yPos, zPos} * radius, 1.f);
            vert.TexCoords = glm::vec4(xSegment, ySegment, 0, 0);
            vert.Normal = glm::vec4(xPos, yPos, zPos, 0);
            vertices.emplace_back(vert);

            if (y >= 1) {
                indices.push_back((y - 1) * (X_SEGMENTS + 1) + x);
                indices.push_back((y - 1) * (X_SEGMENTS + 1) + x + 1);
                indices.push_back(y * (X_SEGMENTS + 1) + x);

                indices.push_back((y - 1) * (X_SEGMENTS + 1) + x + 1);
                indices.push_back(y * (X_SEGMENTS + 1) + x + 1);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
    }

    return {vertices, indices};
}
