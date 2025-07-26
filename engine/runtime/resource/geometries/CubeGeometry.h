#pragma once

#include "Geometry.h"

struct GeometryDescriptor {};

struct CubeGeometry : Geometry {
    static Geometry getDefault();
    static Geometry create(glm::vec3 size = glm::vec3(1.f),
                           GeometryDescriptor desc = {}) {};
};
