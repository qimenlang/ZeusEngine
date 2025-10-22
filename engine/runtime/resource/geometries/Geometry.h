
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "resource/textures/Texture.h"

#define MAX_BONE_INFLUENCE 4

struct GeometryDescriptor {};

// 为保证与GPU端数据对齐，使用vec4存储位置、法线、纹理坐标等属性
struct Vertex {
    glm::vec4 Position;
    glm::vec4 Normal;
    glm::vec4 TexCoords;
    glm::vec4 Tangent;
    glm::vec4 Bitangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Geometry {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // TODO : 需要放在材质里，assimp加载模型时，创建材质
    std::vector<Texture> textures;
};
