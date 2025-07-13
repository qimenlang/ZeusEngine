#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <memory>
#include <string>
#include <vector>

#include "Shader.h"
#include "function/framework/component/mesh_component.h"
#include "function/framework/component/transform_component.h"

struct Texture;

unsigned int TextureFromFile(const char *path, const std::string &directory,
                             bool gamma = false);

class Model {
   private:
    std::string directory;

    std::vector<Texture> textures_loaded;

    std::unique_ptr<MeshComponent> m_mesh_component;
    std::unique_ptr<TransformComponent> m_transform;

    std::weak_ptr<Shader> m_shader;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    SubMesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
                                              aiTextureType type,
                                              std::string typeName);

   public:
    Model(char *path) {
        m_transform = std::make_unique<TransformComponent>();
        m_mesh_component = std::make_unique<MeshComponent>();
        loadModel(path);
    }
    Model(const char *path) {
        m_transform = std::make_unique<TransformComponent>();
        m_mesh_component = std::make_unique<MeshComponent>();
        loadModel(path);
    }
    ~Model() {};

    TransformComponent *transform() { return m_transform.get(); };

    void Draw();
    void setShader(std::shared_ptr<Shader> shader) {
        m_shader = shader;
        m_mesh_component->setShader(shader);
    }
};
