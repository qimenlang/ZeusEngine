#include "asset_manager.h"

#include <include/glad/glad.h>  // holds all OpenGL type declarations

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include "function/framework/component/mesh_component.h"
#include "stb_image.h"

unsigned int TextureFromFile(const char *path, const std::string &directory,
                             bool gamma) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data =
        stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << filename
                  << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

std::vector<Geometry> AssetManager::loadModel(std::string path) {
    std::vector<Geometry> m_primitives;
    Assimp::Importer importer;

    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::cout << importer.GetErrorString() << std::endl;
    } else {
        directory = path.substr(0, path.find_last_of("/"));
        processNode(scene->mRootNode, scene, m_primitives);
    }
    return m_primitives;
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
