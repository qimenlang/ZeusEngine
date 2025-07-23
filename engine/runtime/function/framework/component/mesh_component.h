#pragma once

#include <include/glad/glad.h>  // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "component.h"
#include "function/render/material.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

struct Geometry {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    // material index
};

enum class PrimitiveType {
    POINTS = GL_POINTS,
    LINES = GL_LINES,
    LINE_LOOP = GL_LINE_LOOP,
    LINE_STRIP = GL_LINE_STRIP,
    TRIANGLES = GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN = GL_TRIANGLE_FAN,
};

struct Primitive {
    Geometry geometry;
    // todo:material instance
    std::shared_ptr<materialInstance> material;
    PrimitiveType type;

    // for submesh
    uint32_t indexOffset = 0;
    uint32_t indexCount = 0;

    unsigned int VAO, VBO, EBO;
    Primitive(const Geometry& geometry,
              std::shared_ptr<materialInstance> material);
    void Draw();
};

using PrimitiveList = std::vector<Primitive>;

class MeshComponent : public Component {
   public:
    MeshComponent(const PrimitiveList& primitives)
        : m_primitives(primitives) {};
    MeshComponent() = default;
    ~MeshComponent() = default;

    // virtual void postLoadResource(std::weak_ptr<Object> parent_object)
    // override;
    void tick(float delta_time) override;

    PrimitiveList& primitives() { return m_primitives; };

   private:
    PrimitiveList m_primitives;
};
