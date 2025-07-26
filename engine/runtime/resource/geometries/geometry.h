
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "resource/textures/Texture.h"

#define MAX_BONE_INFLUENCE 4

struct GeometryDescriptor {};

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Geometry {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // TODO : 需要放在材质里，assimp加载模型时，创建材质
    std::vector<Texture> textures;
};
