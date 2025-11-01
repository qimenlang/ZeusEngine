#include <resource/geometries/SphereGeometry.h>

#include <catch2/catch_test_macros.hpp>
#include <iostream>
TEST_CASE("Test Sphere Geometry vertices and indices", "[SphereGeometryTest]") {
    auto sphereGeo = SphereGeometry::create();
    REQUIRE(sphereGeo.checkValidation());
}
