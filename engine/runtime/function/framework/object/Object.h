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
#include "function/framework/component/MeshComponent.h"
#include "function/framework/component/TransformComponent.h"
#include "function/render/Material.h"

struct Texture;

class Object {
   private:
    size_t m_id;
    std::string m_name;
    std::string m_res_path;

    std::vector<Texture> textures_loaded;

    TransformComponent *m_transform;
    std::vector<std::unique_ptr<Component>> m_components;

    // TODO：不应该在object中保存，移到渲染模块
    std::shared_ptr<Material> m_material;

    bool m_enabled = true;

   public:
    Object(const char *path, std::shared_ptr<Material> mat) : Object(mat) {
        m_res_path = *path;
        auto geometrys =
            Zeus::Engine::getInstance().assetManager().loadModel(path);

        PrimitiveList primitives;
        for (const auto &geometry : geometrys) {
            Primitive primitive{geometry, mat->defaultInstance()->duplicate()};
            primitives.emplace_back(primitive);
        }
        auto mesh_component = std::make_unique<MeshComponent>(primitives);
        addComponent(std::move(mesh_component));
    }

    Object(std::string path, std::shared_ptr<Material> mat)
        : Object(path.c_str(), mat) {}

    Object(std::shared_ptr<Material> mat) : Object() { m_material = mat; }

    Object() {
        auto transform = std::make_unique<TransformComponent>();
        m_transform = transform.get();
        addComponent(std::move(transform));
        onTick.add([this](Object *thiz) {
            if (m_material == nullptr) return;
            m_material->use();
            m_material->shader()->setMat4("model",
                                          m_transform->GetModelMatrix());
            m_material->shader()->setMat4(
                "view", Zeus::Engine::getInstance().camera().GetViewMatrix());
            m_material->shader()->setMat4(
                "projection",
                Zeus::Engine::getInstance().camera().GetProjectionMatrix());
        });
    };
    ~Object() = default;

    size_t getId() const { return m_id; }

    void setName(std::string name) { m_name = name; }
    const std::string &getName() const { return m_name; }

    TransformComponent *transform() { return m_transform; };

    Component *addComponent(std::unique_ptr<Component> &&component);
    void removeComponent(Component *);

    template <typename T, typename... Args>
    T *addComponent(Args &&...args) {
        auto component = T::create(*this, std::forward<Args>(args)...);
        return dynamic_cast<T *>(component);
    }

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

    void setEnabled(bool enabled) { m_enabled = enabled; }
    bool isEnabled() const { return m_enabled; }
};
