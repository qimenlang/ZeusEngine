#pragma once

#include "Mesh.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>
#include <memory>

class Mesh;
struct Texture;

unsigned int TextureFromFile(const char *path, const std::string &directory,
                             bool gamma = false);

struct ModelTransform {
  glm::vec3 position = glm::vec3{0.f};
  glm::vec3 rotateAxis = glm::vec3{1.f, 0, 0};
  float rotation = 0.f;
  glm::vec3 scale = glm::vec3{1.f};
};

class Model {
private:
  std::vector<Mesh> meshes;
  std::string directory;

  std::vector<Texture> textures_loaded;

  ModelTransform m_transform;

  std::weak_ptr<Shader> m_shader;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName);

public:
  Model(char *path) { loadModel(path); }
  Model(const char *path) { loadModel(path); }
  ~Model() {};

  ModelTransform transform() { return m_transform; };
  void setPosition(glm::vec3 position) { m_transform.position = position; }
  void setRotation(glm::vec3 rotateAxis, float rotation) {
    m_transform.rotateAxis = rotateAxis;
    m_transform.rotation = rotation;
  }
  void setScale(glm::vec3 scale) { m_transform.scale = scale; }

  glm::mat4 GetModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_transform.position);
    model = glm::rotate(model, m_transform.rotation, m_transform.rotateAxis);
    model = glm::scale(model, m_transform.scale);
    return model;
  }

  void Draw();
  void setShader(std::shared_ptr<Shader> shader) { m_shader = shader; }
};
