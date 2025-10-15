#pragma once
#include <string>

#include "Shader.h"


class ComputeShader : public Shader {
   public:
    ComputeShader(std::string path);
    ComputeShader(const char *path);

   private:
    /* data */
};
