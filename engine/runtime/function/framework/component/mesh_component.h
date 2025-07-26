#pragma once

#include <include/glad/glad.h>  // holds all OpenGL type declarations

#include <string>
#include <vector>

#include "component.h"
#include "function/render/material.h"
#include "resource/geometries/geometry.h"

enum class PrimitiveType {
    POINTS = GL_POINTS,
    LINES = GL_LINES,
    LINE_LOOP = GL_LINE_LOOP,
    LINE_STRIP = GL_LINE_STRIP,
    TRIANGLES = GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN = GL_TRIANGLE_FAN,
};

struct Primitive {
    Geometry geometry;
    // todo:material instance
    std::shared_ptr<MaterialInstance> matInstance;
    PrimitiveType type;

    // for submesh
    uint32_t indexOffset = 0;
    uint32_t indexCount = 0;

    unsigned int VAO, VBO, EBO;
    Primitive(const Geometry& geometry,
              std::shared_ptr<MaterialInstance> material);

    void updateStencilState(StencilState state);
    void Draw();
};

using PrimitiveList = std::vector<Primitive>;

class MeshComponent : public Component {
   public:
    MeshComponent(const PrimitiveList& primitives)
        : m_primitives(primitives) {};
    MeshComponent() = default;
    ~MeshComponent() = default;

    // virtual void postLoadResource(std::weak_ptr<Object> parent_object)
    // override;
    void tick(float delta_time) override;

    PrimitiveList& primitives() { return m_primitives; };

   private:
    PrimitiveList m_primitives;
};
