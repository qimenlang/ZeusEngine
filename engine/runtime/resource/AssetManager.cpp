#include "AssetManager.h"

#include <include/glad/glad.h>  // holds all OpenGL type declarations

#include <iostream>

#include "resource/textures/Texture.h"

#include "function/framework/component/MeshComponent.h"


std::vector<Geometry> AssetManager::loadModel(std::string path) {
    std::vector<Geometry> geometrys;
    Assimp::Importer importer;

    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::cout << importer.GetErrorString() << std::endl;
    } else {
        directory = path.substr(0, path.find_last_of("/"));
        processNode(scene->mRootNode, scene, geometrys);
    }
    return geometrys;
}

void AssetManager::processNode(aiNode *node, const aiScene *scene,
                               std::vector<Geometry> &sub_meshs) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        sub_meshs.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, sub_meshs);
    }
}

Geometry AssetManager::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        // 强制转换为glm::vec3,防止mVertices实际是double类型,导致类型不兼容
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                    mesh->mVertices[i].z);
        if (mesh->HasNormals()) {
            vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                      mesh->mNormals[i].z);
        }

        // 网格是否有纹理坐标？
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
                                         mesh->mTextureCoords[0][i].y);
        } else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        if (mesh->HasTangentsAndBitangents()) {
            vertex.Tangent =
                glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y,
                          mesh->mTangents[i].z);
            vertex.Bitangent =
                glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y,
                          mesh->mBitangents[i].z);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0) {
        auto material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(
            material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(
            material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(),
                        specularMaps.end());
        std::vector<Texture> normalMaps = loadMaterialTextures(
            material, aiTextureType_NORMALS, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        std::vector<Texture> heightMaps = loadMaterialTextures(
            material, aiTextureType_HEIGHT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    std::cout << "vertices size : " << vertices.size() << std::endl;
    return {vertices, indices, textures};
}

std::vector<Texture> AssetManager::loadMaterialTextures(aiMaterial *mat,
                                                        aiTextureType type,
                                                        std::string typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string relativePath = str.C_Str();
        // 替换反斜杠为正斜杠
        std::replace(relativePath.begin(), relativePath.end(), '\\', '/');
        std::cout << "typet:" << type << " ,i: " << i
                  << "path:" << relativePath.c_str() << std::endl;

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data(),
                            relativePath.c_str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(relativePath.c_str(), directory);
            texture.type = typeName;
            // 材质路径是相对于模型文件的本地路径
            texture.path = relativePath.c_str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // 添加到已加载的纹理中
        }
    }
    return textures;
}
