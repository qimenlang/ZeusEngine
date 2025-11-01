
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

struct BoundingBox {
    glm::vec3 min;
    glm::vec3 max;
};

class Geometry {
   private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    // TODO : 需要放在材质里，assimp加载模型时，创建材质
    std::vector<Texture> m_textures;
    BoundingBox m_boundingBox;
    void updateBoundingBox();

   public:
    Geometry(std::vector<Vertex> verts = {},
             std::vector<unsigned int> idxs = {},
             std::vector<Texture> texs = {})
        : m_vertices(std::move(verts)),
          m_indices(std::move(idxs)),
          m_textures(std::move(texs)) {
        updateBoundingBox();
    }

    // 拷贝构造函数
    Geometry(const Geometry& other)
        : m_vertices(other.vertices()),
          m_indices(other.indices()),
          m_textures(other.textures()),
          m_boundingBox(other.boundingBox()) {}

    // 移动构造函数
    Geometry(Geometry&& other)
        : m_vertices(std::move(other.vertices())),
          m_indices(std::move(other.indices())),
          m_textures(std::move(other.textures())),
          m_boundingBox(std::move(other.boundingBox())) {}

    const std::vector<Vertex>& vertices() const { return m_vertices; }

    void setVertices(const std::vector<Vertex>& vertices) {
        m_vertices = vertices;
        updateBoundingBox();
    }
    const std::vector<unsigned int>& indices() const { return m_indices; };

    void setIndices(const std::vector<unsigned int>& indices) {
        m_indices = indices;
    }

    const std::vector<Texture>& textures() const { return m_textures; }
    void setTextures(const std::vector<Texture>& textures) {
        m_textures = textures;
    }

    const BoundingBox& boundingBox() const { return m_boundingBox; }
    bool checkValidation();
};
