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

std::shared_ptr<materialInstance> Material::defaultInstance() {
    if (!m_defaultInstance) {
        m_defaultInstance =
            std::shared_ptr<materialInstance>(new materialInstance(this));
    }
    return m_defaultInstance;
};

Material::Material(const char *vertexPath, const char *fragmentPath) {
    m_shader = std::make_unique<Shader>(vertexPath, fragmentPath);
}

std::shared_ptr<materialInstance> materialInstance::duplicate() {
    return std::shared_ptr<materialInstance>(new materialInstance(m_material));
}

// utility uniform functions
void materialInstance::setBool(const std::string &name, bool value) {
    m_material->m_shader->setBool(name, value);
}

void materialInstance::setInt(const std::string &name, int value) {
    m_material->m_shader->setInt(name, value);
}
void materialInstance::setFloat(const std::string &name, float value) {
    m_material->m_shader->setFloat(name, value);
}
void materialInstance::setMat4(const std::string &name, glm::mat4 mat) {
    m_material->m_shader->setMat4(name, mat);
}
void materialInstance::setVec3(const std::string &name, glm::vec3 vec) {
    m_material->m_shader->setVec3(name, vec);
}

void materialInstance::setDepthTest(bool enable) { m_depthTest = enable; }

bool materialInstance::depthTest() const { return m_depthTest; }

void materialInstance::setDepthWirte(bool enable) { m_depthWirte = enable; }

bool materialInstance::depthWrite() const { return m_depthWirte; }
