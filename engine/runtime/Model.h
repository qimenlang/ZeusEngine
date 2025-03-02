#pragma once

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

class Mesh;
struct Texture;
class Shader;

unsigned int TextureFromFile(const char *path, const std::string &directory,
                             bool gamma = false);

class Model {
private:
  std::vector<Mesh> meshes;
  std::string directory;

  std::vector<Texture> textures_loaded;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName);

public:
  Model(char *path) { loadModel(path); }
  Model(const char *path) { loadModel(path); }
  ~Model() {};
  void Draw(Shader shader);
};
