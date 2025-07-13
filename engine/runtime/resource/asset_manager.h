#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <string>
#include <vector>

#include "Shader.h"
#include "function/framework/component/mesh_component.h"

struct SubMesh;
struct Texture;

unsigned int TextureFromFile(const char *path, const std::string &directory,
                             bool gamma = false);

class AssetManager {
   public:
    AssetManager(/* args */) {};
    ~AssetManager() {};

    std::vector<SubMesh> loadModel(std::string path);

   private:
    void processNode(aiNode *node, const aiScene *scene,
                     std::vector<SubMesh> &m_sub_meshs);
    SubMesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
                                              aiTextureType type,
                                              std::string typeName);

   private:
    std::string directory;
    std::vector<Texture> textures_loaded;
};
