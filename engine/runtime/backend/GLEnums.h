#pragma once
#include <cstdint>

enum class CullingMode : uint8_t {
    NONE,  // 正面背面均不剔除
    FRONT,
    BACK,
    FRONTANDBACK
};