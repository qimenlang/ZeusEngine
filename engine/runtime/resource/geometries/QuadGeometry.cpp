#pragma once

#include "QuadGeometry.h"

#include <iostream>

// clang-format off
float quadVertices[] = {
    // positions          // texture Coords
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};
// clang-format on

Geometry QuadGeometry::getDefault() {
    Geometry cube;
    const int stride = 5;
    int cubeVertexSize = sizeof(quadVertices) / sizeof(float) / stride;

    for (int i = 0; i < cubeVertexSize; i++) {
        Vertex vertex;
        vertex.Position = {quadVertices[i * stride + 0],
                           quadVertices[i * stride + 1],
                           quadVertices[i * stride + 2]};
        vertex.TexCoords = {quadVertices[i * stride + 3],
                            quadVertices[i * stride + 4]};
        cube.vertices.emplace_back(vertex);
    }
    std::cout << "QuadGeometry create" << std::endl;
    return cube;
}