#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "Shader.h"
#include "component.h"

#include <include/glad/glad.h>  // holds all OpenGL type declarations


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
};

enum class PrimitiveType {
    POINTS = GL_POINTS,
    LINES = GL_LINES,
    LINE_LOOP = GL_LINE_LOOP,
    LINE_STRIP = GL_LINE_STRIP,
    TRIANGLES= GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN = GL_TRIANGLE_FAN,
};

struct Primitive  {
    PrimitiveType type;
    Geometry geometry;
    // for submesh
    uint32_t indexOffset = 0;
    uint32_t indexCount = 0;

    unsigned int VAO, VBO, EBO;
    void setup();
    void Draw(unsigned int shaderID);
};

class MeshComponent : public Component {
   public:
    MeshComponent() {};
    ~MeshComponent() {};

    virtual void postLoadResource(std::weak_ptr<Object> parent_object) override;
    void tick(float delta_time) override;
    void setShader(std::shared_ptr<Shader> shader) { m_shader = shader; }
    void addGeometry(const Geometry& geometry);

   private:
    std::vector<Primitive> m_primitives;
    void setup();
    std::weak_ptr<Shader> m_shader;
};
