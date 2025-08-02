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

constexpr GLenum getCompareFunc(SamplerCompareFunc const func) noexcept {
    switch (func) {
        case SamplerCompareFunc::LE:
            return GL_LEQUAL;
        case SamplerCompareFunc::GE:
            return GL_GEQUAL;
        case SamplerCompareFunc::L:
            return GL_LESS;
        case SamplerCompareFunc::G:
            return GL_GREATER;
        case SamplerCompareFunc::E:
            return GL_EQUAL;
        case SamplerCompareFunc::NE:
            return GL_NOTEQUAL;
        case SamplerCompareFunc::A:
            return GL_ALWAYS;
        case SamplerCompareFunc::N:
            return GL_NEVER;
    }
    // should never happen
    return GL_LEQUAL;
}
