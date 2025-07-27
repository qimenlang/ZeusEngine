#pragma once

#include <include/glad/glad.h>  // holds all OpenGL type declarations

#include "GLEnums.h"

constexpr GLenum getCullingMode(CullingMode mode) {
    switch (mode) {
        case CullingMode::NONE:
            return GL_FRONT_AND_BACK;
        case CullingMode::FRONT:
            return GL_FRONT;
        case CullingMode::BACK:
            return GL_BACK;
        case CullingMode::FRONTANDBACK:
            return GL_FRONT_AND_BACK;
    }
    return GL_FRONT_AND_BACK;
}