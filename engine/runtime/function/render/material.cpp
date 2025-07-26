#include "material.h"

#include "include/glad/glad.h"

// glad.h must before glfw3.h
#include <GLFW/glfw3.h>
std::shared_ptr<Material> Material::create(const char *vertexPath,
                                           const char *fragmentPath) {
    // 不能使用make_shared, make_shared不能访问private 构造函数
    auto material =
        std::shared_ptr<Material>(new Material(vertexPath, fragmentPath));
    return material;
}

std::shared_ptr<MaterialInstance> Material::defaultInstance() {
    if (!m_defaultInstance) {
        m_defaultInstance =
            std::shared_ptr<MaterialInstance>(new MaterialInstance(this));
    }
    return m_defaultInstance;
};

Material::Material(const char *vertexPath, const char *fragmentPath) {
    m_shader = std::make_unique<Shader>(vertexPath, fragmentPath);
}

std::shared_ptr<MaterialInstance> MaterialInstance::duplicate() {
    return std::shared_ptr<MaterialInstance>(new MaterialInstance(m_material));
}

// utility uniform functions
void MaterialInstance::setBool(const std::string &name, bool value) {
    m_material->m_shader->setBool(name, value);
}

void MaterialInstance::setInt(const std::string &name, int value) {
    m_material->m_shader->setInt(name, value);
}
void MaterialInstance::setFloat(const std::string &name, float value) {
    m_material->m_shader->setFloat(name, value);
}
void MaterialInstance::setMat4(const std::string &name, glm::mat4 mat) {
    m_material->m_shader->setMat4(name, mat);
}
void MaterialInstance::setVec3(const std::string &name, glm::vec3 vec) {
    m_material->m_shader->setVec3(name, vec);
}

void MaterialInstance::setDepthTest(bool enable) { m_depthTest = enable; }

bool MaterialInstance::depthTest() const { return m_depthTest; }

void MaterialInstance::setDepthWirte(bool enable) { m_depthWirte = enable; }

bool MaterialInstance::depthWrite() const { return m_depthWirte; }
