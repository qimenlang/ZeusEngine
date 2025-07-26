#pragma once
#include <include/glad/glad.h>

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

enum class CompareOperation : unsigned int {
    LESS = GL_LESS,
    LEQUAL = GL_LEQUAL,
    EQUAL = GL_EQUAL,
    GREATER = GL_GREATER,
    GEQUAL = GL_GEQUAL,
    NOTEQUAL = GL_NOTEQUAL,
    ALWAYS = GL_ALWAYS,
    NEVER = GL_NEVER
};
enum class StencilOperation : uint8_t {
    KEEP = GL_KEEP,        // Keeps the current value.
    ZERO = GL_ZERO,        // Sets the value to 0.
    REPLACE = GL_REPLACE,  // Sets the value to the stencil reference value.
    INCR = GL_INCR,  // Increments the current value. Clamps to the maximum
                     // representable unsigned value.
    INCR_WRAP = GL_INCR_WRAP,  // Increments the current value. Wraps value to
                               // zero when incrementing the maximum
                               // representable unsigned value.
    DECR = GL_DECR,            // Decrements the current value. Clamps to 0.
    DECR_WRAP = GL_DECR_WRAP,  // Decrements the current value. Wraps value to
                               // the maximum representable unsigned value when
                               // decrementing a value of zero.
    INVERT = GL_INVERT,        // Bitwise inverts the current value.
};

struct StencilState {
    bool stencilWrite = false;
    uint8_t readMask = 0xFF;
    uint8_t writeMask = 0xFF;
    uint8_t ref = 0;
    CompareOperation compareFunc = CompareOperation::ALWAYS;
    StencilOperation stencilFail = StencilOperation::KEEP;
    StencilOperation depthFail = StencilOperation::KEEP;
    StencilOperation stencilDepthSucc = StencilOperation::KEEP;
};

class MaterialInstance {
   private:
    Material *m_material;

    friend class Material;
    MaterialInstance(Material *mat) { m_material = mat; }

    bool m_depthTest = true;
    bool m_depthWirte = true;
    StencilState m_stencilState;

   public:
    std::shared_ptr<MaterialInstance> duplicate();

    unsigned int shaderID() { return m_material->shaderID(); }

    StencilState getStencillState() { return m_stencilState; }
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

    void setStencilWrite(bool write) { m_stencilState.stencilWrite = write; }
    void setStencilReadMask(uint8_t readMask) {
        m_stencilState.readMask = readMask;
    }
    void setStencilWriteMask(uint8_t writeMask) {
        m_stencilState.writeMask = writeMask;
    }
    void setStencilReferenceValue(uint8_t ref) { m_stencilState.ref = ref; }
    void setStencilCompareFunc(CompareOperation op) {
        m_stencilState.compareFunc = op;
    };
    void setStencilOpStencilFail(StencilOperation op) {
        m_stencilState.stencilFail = op;
    }
    void setStencilOpDepthFail(StencilOperation op) {
        m_stencilState.depthFail = op;
    }
    void setStencilOpStencilDepthSucc(StencilOperation op) {
        m_stencilState.stencilDepthSucc = op;
    }
};
