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
#include "function/render/material.h"

struct Texture;

class Object {
   private:
    size_t m_id;
    std::string m_name;
    std::string m_res_path;

    std::vector<Texture> textures_loaded;

    TransformComponent *m_transform;
    std::vector<std::unique_ptr<Component>> m_components;

    std::shared_ptr<Material> m_material;

   public:
    Object(const char *path, std::shared_ptr<Material> mat) : Object() {
        m_res_path = *path;
        auto geometrys =
            Zeus::Engine::getInstance().assetManager().loadModel(path);

        m_material = mat;

        PrimitiveList primitives;
        for (const auto &geometry : geometrys) {
            Primitive primitive{geometry, mat->defaultInstance()->duplicate()};
            primitives.emplace_back(primitive);
        }
        auto mesh_component = std::make_unique<MeshComponent>(primitives);
        addComponent(std::move(mesh_component));
    }

    Object() {
        auto transform = std::make_unique<TransformComponent>();
        m_transform = transform.get();
        addComponent(std::move(transform));
    }
    ~Object() = default;

    size_t getId() const { return m_id; }

    void setName(std::string name) { m_name = name; }
    const std::string &getName() const { return m_name; }

    TransformComponent *transform() { return m_transform; };

    Component *addComponent(std::unique_ptr<Component> &&component);
    void removeComponent(Component *);

    template <typename T>
    T *getComponent() {
        for (auto &component : m_components) {
            if (dynamic_cast<T *>(component.get()))
                return dynamic_cast<T *>(component.get());
        }
        return nullptr;
    };

    std::vector<Component *> getAllComponents() {
        std::vector<Component *> results;
        for (auto &comp : m_components) {
            results.push_back(comp.get());
        }
        return results;
    }

    void tick();

    Event<void(Object *thiz)> onTick;
};
