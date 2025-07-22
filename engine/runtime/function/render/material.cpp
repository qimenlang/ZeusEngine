#include "material.h"

Material::Material(const char *vertexPath, const char *fragmentPath) {
    m_shader = std::make_unique<Shader>(vertexPath, fragmentPath);
}

std::shared_ptr<Material> Material::cteate(const char *vertexPath,
                                           const char *fragmentPath) {
    auto material = std::make_shared<Material>(vertexPath, fragmentPath);
    return material;
}