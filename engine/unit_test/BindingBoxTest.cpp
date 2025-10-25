#include <resource/geometries/Geometry.h>

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

TEST_CASE("Generate bounding box ", "[BindingBoxTest]") {
    std::vector<Vertex> vertices = {
        {{1.0f, 2.0f, 3.0f, 0.0f}},     // Vertex 1
        {{-1.0f, -2.0f, -3.0f, 0.0f}},  // Vertex 2
        {{4.0f, 5.0f, 6.0f, 0.0f}},     // Vertex 3
        {{-4.0f, -5.0f, -6.0f, 0.0f}}   // Vertex 4
    };

    Geometry geo;

    SECTION("Test No Vertices") {
        REQUIRE(geo.boundingBox().min == glm::vec3(0.f));
        REQUIRE(geo.boundingBox().max == glm::vec3(0.f));
    }

    SECTION("Test Regular Min Max when construction") {
        Geometry tmpGeo(vertices);
        std::cout << glm::to_string(tmpGeo.boundingBox().min).c_str();
        REQUIRE(tmpGeo.boundingBox().min == glm::vec3(-4.0f, -5.0, -6.0f));
        REQUIRE(tmpGeo.boundingBox().max == glm::vec3(4.0f, 5.0, 6.0f));
    }

    SECTION("Test Regular Min Max when set vertices") {
        geo.setVertices(vertices);
        std::cout << glm::to_string(geo.boundingBox().min).c_str();
        REQUIRE(geo.boundingBox().min == glm::vec3(-4.0f, -5.0, -6.0f));
        REQUIRE(geo.boundingBox().max == glm::vec3(4.0f, 5.0, 6.0f));
    }

    SECTION("Test Regular Min Max when copy construction") {
        geo.setVertices(vertices);
        auto tmpGeo = geo;
        REQUIRE(tmpGeo.boundingBox().min == glm::vec3(-4.0f, -5.0, -6.0f));
        REQUIRE(tmpGeo.boundingBox().max == glm::vec3(4.0f, 5.0, 6.0f));
    }
}
