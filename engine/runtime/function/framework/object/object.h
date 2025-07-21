#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "Engine.h"
#include "core/event.h"
#include "function/framework/component/mesh_component.h"
#include "function/framework/component/transform_component.h"
#include "function/render/Shader.h"

struct Texture;

class Object {
   private:
    size_t m_id;
    std::string m_name;
    std::string m_res_path;

    std::vector<Texture> textures_loaded;

    std::unique_ptr<MeshComponent> m_mesh_component;
    std::unique_ptr<TransformComponent> m_transform;

    std::weak_ptr<Shader> m_shader;

   public:
    Object(const char *path, const char *vs, const char *fs) {
        m_transform = std::make_unique<TransformComponent>();
        m_res_path = *path;
        auto geometrys =
            Zeus::Engine::getInstance().assetManager().loadModel(path);

        auto shader = std::make_shared<Shader>(vs, fs);
        m_shader = shader;

        PrimitiveList primitives;
        for (const auto &geometry : geometrys) {
            Primitive primitive{geometry, shader};
            primitives.emplace_back(primitive);
        }
        m_mesh_component = std::make_unique<MeshComponent>(primitives);
    }

    ~Object() {};

    size_t getId() const { return m_id; }

    void setName(std::string name) { m_name = name; }
    const std::string &getName() const { return m_name; }

    TransformComponent *transform() { return m_transform.get(); };
    MeshComponent *mesh_component() { return m_mesh_component.get(); };

    void tick();

    Event<void(Object *thiz)> onTick;
};
