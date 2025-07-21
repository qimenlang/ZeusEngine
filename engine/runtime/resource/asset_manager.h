#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <string>
#include <vector>

#include "function/framework/component/mesh_component.h"
#include "function/render/Shader.h"


struct Geometry;
struct Texture;

unsigned int TextureFromFile(const char *path, const std::string &directory,
                             bool gamma = false);

class AssetManager {
   public:
    AssetManager(/* args */) {};
    ~AssetManager() {};

    std::vector<Geometry> loadModel(std::string path);

   private:
    void processNode(aiNode *node, const aiScene *scene,
                     std::vector<Geometry> &m_primitives);
    Geometry processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
                                              aiTextureType type,
                                              std::string typeName);

   private:
    std::string directory;
    std::vector<Texture> textures_loaded;
};
