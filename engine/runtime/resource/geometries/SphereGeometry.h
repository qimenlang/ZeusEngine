#pragma once

#include "Geometry.h"

struct SphereGeometry : Geometry {
    static Geometry getDefault() {};
    static Geometry create(float radius = 1.f, GeometryDescriptor desc = {});
};
