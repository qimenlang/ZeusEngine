#pragma once
#include <memory>

#include "Shader.h"

class Material {
   public:
    Material(const char *vertexPath, const char *fragmentPath);

    static std::shared_ptr<Material> cteate(const char *vertexPath,
                                            const char *fragmentPath);

   private:
    std::unique_ptr<Shader> m_shader;
};