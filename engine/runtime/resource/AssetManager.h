#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <string>
#include <vector>

#include "function/framework/component/MeshComponent.h"

struct Geometry;
struct Texture;

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
