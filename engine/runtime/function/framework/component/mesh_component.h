#pragma once
#include <include/glad/glad.h>  // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "Shader.h"
#include "component.h"

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

struct SubMesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
};

struct SubMeshRenderable : public SubMesh {
    unsigned int VAO, VBO, EBO;
    void setupMesh();
    void Draw(unsigned int shaderID);
};

class MeshComponent : public Component {
   public:
    MeshComponent() {};
    ~MeshComponent() {};

    virtual void postLoadResource(std::weak_ptr<Object> parent_object) override;
    void tick(float delta_time) override;
    void setShader(std::shared_ptr<Shader> shader) { m_shader = shader; }
    void addSubMesh(const SubMesh& sub_mesh);

   private:
    std::vector<SubMeshRenderable> m_sub_meshs;
    void setupMesh();
    std::weak_ptr<Shader> m_shader;
};
