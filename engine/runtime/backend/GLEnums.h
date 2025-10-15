#pragma once
#include <cstdint>

enum class CullingMode : uint8_t {
    NONE,  // 正面背面均不剔除
    FRONT,
    BACK,
    FRONTANDBACK
};

//! comparison function for the depth / stencil sampler
enum class SamplerCompareFunc : uint8_t {
    // don't change the enums values
    LE = 0,  //!< Less or equal
    GE,      //!< Greater or equal
    L,       //!< Strictly less than
    G,       //!< Strictly greater than
    E,       //!< Equal
    NE,      //!< Not equal
    A,       //!< Always. Depth / stencil testing is deactivated.
    N        //!< Never. The depth / stencil test always fails.
};
