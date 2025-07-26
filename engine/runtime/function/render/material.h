#pragma once
#include <memory>

#include "Shader.h"

class MaterialInstance;
class Material {
   public:
    friend class MaterialInstance;
    static std::shared_ptr<Material> create(const char *vertexPath,
                                            const char *fragmentPath);
    void use() { m_shader->use(); }
    std::shared_ptr<MaterialInstance> defaultInstance();

    unsigned int shaderID() { return m_shader->ID; }

    Shader *shader() { return m_shader.get(); }

   private:
    Material(const char *vertexPath, const char *fragmentPath);
    std::unique_ptr<Shader> m_shader;
    std::shared_ptr<MaterialInstance> m_defaultInstance;
};

class MaterialInstance {
   private:
    Material *m_material;

    friend class Material;
    MaterialInstance(Material *mat) { m_material = mat; }

    bool m_depthTest = true;
    bool m_depthWirte = true;

   public:
    std::shared_ptr<MaterialInstance> duplicate();

    unsigned int shaderID() { return m_material->shaderID(); }

    // utility uniform functions
    void use() {
        if (m_material) m_material->use();
    }

    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setMat4(const std::string &name, glm::mat4 mat);
    void setVec3(const std::string &name, glm::vec3 vec);

    void setDepthTest(bool enable);
    bool depthTest() const;
    void setDepthWirte(bool enable);
    bool depthWrite() const;
};
