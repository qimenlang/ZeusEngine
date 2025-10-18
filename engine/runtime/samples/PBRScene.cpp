#include "PBRScene.h"

#include <imgui.h>

#include <memory>

#include "Engine.h"
#include "function/framework/component/MeshComponent.h"
#include "function/framework/object/Object.h"
#include "function/render/Material.h"
#include "resource/geometries/SphereGeometry.h"

struct subsurfaceParams {
    glm::vec3 color = glm::vec3(0.8f, 0.0f, 0.0f);
    float power = 10.0f;
    float thickness = 0.5f;
} subsurfacePara;

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
        std::string(ZEUS_ROOT_DIR).append("/shader/brdf.fs");

    auto brdfMat = Material::create(vs_path.c_str(), pbr_fs_path.c_str());
    brdfMat->shader()->use();
    brdfMat->shader()->setVec3("mat.albedo", glm::vec3(0.5f, 0.0f, 0.0f));
    brdfMat->shader()->setFloat("mat.ao", 1.0f);

    auto createSphere =
        [&](std::shared_ptr<Material> mat) -> std::unique_ptr<Object> {
        auto sphere = std::make_unique<Object>(mat);
        auto sphereGeo = SphereGeometry::create(0.48);
        Primitive spherePrimitive = {sphereGeo,
                                     mat->defaultInstance()->duplicate()};
        sphere->addComponent(std::move(
            std::make_unique<MeshComponent>(PrimitiveList{spherePrimitive})));
        return std::move(sphere);
    };

    // for PBR Test
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            auto sphere = createSphere(brdfMat);
            sphere->setEnabled(false);
            sphere->transform()->setPosition({i - 3.5, j - 3.5, -5});
            m_PBRSpheres.emplace_back(std::move(sphere));
        }
    }

    std::string bsdf_fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/bsdf.fs");

    auto bsdfMat = Material::create(vs_path.c_str(), bsdf_fs_path.c_str());
    bsdfMat->shader()->use();
    bsdfMat->shader()->setVec3("mat.albedo", glm::vec3(0.0f, 0.5f, 0.0f));
    bsdfMat->shader()->setFloat("mat.ao", 1.0f);
    bsdfMat->shader()->setVec3("subsurface.color", glm::vec3(0.8f, 0.0f, 0.0f));
    bsdfMat->shader()->setFloat("subsurface.power", 10.0f);
    bsdfMat->shader()->setFloat("subsurface.thickness", 0.5f);

    // for SSS Test
    auto sphere = createSphere(bsdfMat);
    m_SSSSpheres.emplace_back(std::move(sphere));
}

static void gui() {
    ImGui::Begin("Subsurface Controls");
    ImGui::SliderFloat("Thickness", &subsurfacePara.thickness, 0.0f, 1.0f);
    ImGui::SliderFloat("Subsurface power", &subsurfacePara.power, 1.0f, 24.0f);
    ImGui::ColorEdit3("Subsurface color", &subsurfacePara.color[0]);
    ImGui::End();
}

void PBRScene::update() {
    gui();

    auto lightColor = glm::vec3(300.0f);
    auto &lightShader =
        m_lights[0]->getComponent<MeshComponent>()->primitives()[0].matInstance;
    lightShader->use();
    for (auto &light : m_lights) {
        lightShader->setVec3("lightColor", lightColor);
        light->tick();
    }

    auto &pbrShader = m_PBRSpheres[0]
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

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            auto &sphere = m_PBRSpheres[i * 7 + j];
            // clamp to 0.05-1.0; 0粗糙度绝对光滑，看起来不太自然
            pbrShader->setFloat("mat.roughness",
                                glm::clamp(float(i) / 7.f, 0.05f, 1.0f));
            pbrShader->setFloat("mat.metallic", float(j) / 7.f);
            sphere->tick();
        }
    }

    auto &sssShader = m_SSSSpheres[0]
                          ->getComponent<MeshComponent>()
                          ->primitives()[0]
                          .matInstance;
    sssShader->use();
    sssShader->setVec3("camPos", Zeus::Engine::getInstance().camera().Position);
    for (int i = 0; i < m_lights.size(); i++) {
        sssShader->setVec3("lights[" + std::to_string(i) + "].pos",
                           m_lights[i]->transform()->position());
        sssShader->setVec3("lights[" + std::to_string(i) + "].color",
                           lightColor);
    }

    for (auto &sphere : m_SSSSpheres) {
        sssShader->setFloat("mat.roughness", 0.5f);
        sssShader->setFloat("mat.metallic", 0.5f);
        sssShader->setFloat("subsurface.thickness", subsurfacePara.thickness);
        sssShader->setFloat("subsurface.power", subsurfacePara.power);
        sssShader->setVec3("subsurface.color", subsurfacePara.color);

        sphere->tick();
    }
}