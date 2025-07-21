#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "Engine.h"
#include "function/render/Shader.h"
#include "core/event.h"
#include "function/framework/component/mesh_component.h"
#include "function/framework/component/transform_component.h"

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
    Object(const char *path) {
        m_transform = std::make_unique<TransformComponent>();
        m_mesh_component = std::make_unique<MeshComponent>();
        m_res_path = *path;
        auto geometrys =
            Zeus::Engine::getInstance().assetManager().loadModel(path);
        for (const auto &geometry : geometrys)
            m_mesh_component->addGeometry(geometry);
    }
    Object(char *path) {
        m_transform = std::make_unique<TransformComponent>();
        m_mesh_component = std::make_unique<MeshComponent>();
        m_res_path = *path;
        auto geometrys =
            Zeus::Engine::getInstance().assetManager().loadModel(path);
        for (const auto &geometry : geometrys)
            m_mesh_component->addGeometry(geometry);
    }
    ~Object(){};

    size_t getId() const { return m_id; }

    void setName(std::string name) { m_name = name; }
    const std::string &getName() const { return m_name; }

    TransformComponent *transform() { return m_transform.get(); };

    void tick();

    Event<void(Object *thiz)> onTick;

    void setShader(std::shared_ptr<Shader> shader) {
        m_shader = shader;
        m_mesh_component->setShader(shader);
    }
};
