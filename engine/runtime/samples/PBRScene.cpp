#include "PBRScene.h"

#include <memory>

#include "Engine.h"
#include "function/framework/component/MeshComponent.h"
#include "function/framework/object/Object.h"
#include "function/render/Material.h"
#include "resource/geometries/SphereGeometry.h"

PBRScene::PBRScene() : Scene() {
    std::cout << "PBRScene initialized." << std::endl;
}

PBRScene::~PBRScene() { std::cout << "PBRScene destroyed." << std::endl; }

void PBRScene::init() {
    std::cout << "PBRScene init called." << std::endl;
    std::string vs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/default.vs");
    std::string ls_path = std::string(ZEUS_ROOT_DIR).append("/shader/light.fs");

    auto light_mat = Material::create(vs_path.c_str(), ls_path.c_str());

    std::string lightCubePath =
        std::string(ZEUS_ROOT_DIR).append("/model/cube.obj");
    auto addLight = [&](glm::vec3 pos) -> std::unique_ptr<Object> {
        auto light = std::make_unique<Object>(lightCubePath.c_str(), light_mat);
        light->transform()->setPosition(pos);
        light->transform()->setScale(glm::vec3(0.04f));
        return std::move(light);
    };

    m_lights.emplace_back(addLight({-1.0, -1.0, 2.0}));
    m_lights.emplace_back(addLight({-1.0, 1.0, 2.0}));
    m_lights.emplace_back(addLight({1.0, -1.0, 2.0}));
    m_lights.emplace_back(addLight({1.0, 1.0, 2.0}));

    std::string pbr_fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/pbr.fs");

    auto pbr_sphere_mat =
        Material::create(vs_path.c_str(), pbr_fs_path.c_str());
    pbr_sphere_mat->shader()->use();
    pbr_sphere_mat->shader()->setVec3("mat.albedo",
                                      glm::vec3(0.5f, 0.0f, 0.0f));
    pbr_sphere_mat->shader()->setFloat("mat.ao", 1.0f);

    auto createSphere = [&]() -> std::unique_ptr<Object> {
        auto sphere = std::make_unique<Object>(pbr_sphere_mat);
        auto sphereGeo = SphereGeometry::create(0.48);
        Primitive spherePrimitive = {
            sphereGeo, pbr_sphere_mat->defaultInstance()->duplicate()};
        sphere->addComponent(std::move(
            std::make_unique<MeshComponent>(PrimitiveList{spherePrimitive})));
        return std::move(sphere);
    };

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            auto sphere = createSphere();
            sphere->transform()->setPosition({i - 5, j - 5, -10});
            m_spheres.emplace_back(std::move(sphere));
        }
    }
}

void PBRScene::update() {
    auto lightColor = glm::vec3(300.0f);
    auto &lightShader =
        m_lights[0]->getComponent<MeshComponent>()->primitives()[0].matInstance;
    lightShader->use();
    for (auto &light : m_lights) {
        lightShader->setVec3("lightColor", lightColor);
        light->tick();
    }

    auto &pbrShader = m_spheres[0]
                          ->getComponent<MeshComponent>()
                          ->primitives()[0]
                          .matInstance;

    pbrShader->use();
    pbrShader->setVec3("camPos", Zeus::Engine::getInstance().camera().Position);
    for (int i = 0; i < m_lights.size(); i++) {
        pbrShader->setVec3("lights[" + std::to_string(i) + "].pos",
                           m_lights[i]->transform()->position());
        pbrShader->setVec3("lights[" + std::to_string(i) + "].color",
                           lightColor);
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            auto &sphere = m_spheres[i * 10 + j];
            pbrShader->setFloat("mat.roughness", float(i) / 10.0);
            pbrShader->setFloat("mat.metallic", float(j) / 10.0);
            sphere->tick();
        }
    }
}