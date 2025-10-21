#pragma once

#include "QuadGeometry.h"

#include <iostream>

// clang-format off
// float quadVertices[] = {
//     // positions          // texture Coords
//     0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
//     0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
//     1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

//     0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
//     1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
//     1.0f,  0.5f,  0.0f,  1.0f,  0.0f
// };

// for test computer ssbo

float quadVertices[] = {
    // positions          // texture Coords
    0.4f,  0.4f,  0.0f,  0.0f,  0.0f,
    0.6f, 0.4f,  0.0f,  0.0f,  1.0f,
    0.4f, 0.6f,  0.0f,  1.0f,  1.0f,

    0.6f,  0.4f,  0.0f,  0.0f,  0.0f,
    0.55f, 0.55f,  0.0f,  1.0f,  1.0f,
    0.4f,  0.6f,  0.0f,  1.0f,  0.0f
};

float screenQuadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
    };
// clang-format on

Geometry QuadGeometry::getDefault(QuadGeometryType type) {
    Geometry cube;

    auto createNormalQuad = [&]() {
        const int stride = 5;
        int cubeVertexSize = sizeof(quadVertices) / sizeof(float) / stride;

        for (int i = 0; i < cubeVertexSize; i++) {
            Vertex vertex;
            vertex.Position = {quadVertices[i * stride + 0],
                               quadVertices[i * stride + 1],
                               quadVertices[i * stride + 2], 0};
            vertex.TexCoords = {quadVertices[i * stride + 3],
                                quadVertices[i * stride + 4], 0, 0};
            cube.vertices.emplace_back(vertex);
        }
        std::cout << "QuadGeometry create" << std::endl;
    };

    auto createScreenQuad = [&]() {
        const int stride = 4;
        int cubeVertexSize =
            sizeof(screenQuadVertices) / sizeof(float) / stride;

        for (int i = 0; i < cubeVertexSize; i++) {
            Vertex vertex;
            vertex.Position = {screenQuadVertices[i * stride + 0],
                               screenQuadVertices[i * stride + 1], 0.f, 0.f};
            vertex.TexCoords = {screenQuadVertices[i * stride + 2],
                                screenQuadVertices[i * stride + 3], 0, 0};
            cube.vertices.emplace_back(vertex);
        }
        std::cout << "ScreenQuadGeometry create" << std::endl;
    };

    switch (type) {
        case QuadGeometryType::NormalQuad:
            createNormalQuad();
            break;
        case QuadGeometryType::ScreenQuad:
            createScreenQuad();
            break;

        default:
            break;
    }
    return cube;
}