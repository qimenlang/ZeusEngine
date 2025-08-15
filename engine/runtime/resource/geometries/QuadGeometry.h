#pragma once

#include "Geometry.h"

enum class QuadGeometryType {
    NormalQuad,
    ScreenQuad,
};

struct QuadGeometry : Geometry {
    static Geometry getDefault(
        QuadGeometryType type = QuadGeometryType::NormalQuad);
    static Geometry create(glm::vec3 size = glm::vec3(1.f),
                           GeometryDescriptor desc = {}) {};
};
