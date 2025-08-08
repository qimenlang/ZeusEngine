#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <map>
#include <string>
#include <vector>

#include "function/framework/component/MeshComponent.h"

struct Geometry;
struct Texture;

class AssetManager {
   public:
    AssetManager(/* args */) {
        m_textureMap["diffuse"] = aiTextureType_DIFFUSE;
        m_textureMap["specular"] = aiTextureType_SPECULAR;
        m_textureMap["height"] = aiTextureType_HEIGHT;
        // 切线空间法线贴图
        m_textureMap["normals"] = aiTextureType_NORMALS;
        m_textureMap["albedo"] = aiTextureType_BASE_COLOR;
        // 相机空间法线贴图
        m_textureMap["normalCamera"] = aiTextureType_NORMAL_CAMERA;
        m_textureMap["metalness"] = aiTextureType_METALNESS;
        m_textureMap["roughness"] = aiTextureType_DIFFUSE_ROUGHNESS;
        m_textureMap["ao"] = aiTextureType_AMBIENT_OCCLUSION;
    };
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
    std::map<std::string, aiTextureType> m_textureMap;
};
