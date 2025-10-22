#include "PBRScene.h"

#include <imgui.h>

#include <memory>

#include "Engine.h"
#include "function/framework/component/MeshComponent.h"
#include "function/framework/object/Object.h"
#include "function/render/Material.h"
#include "resource/geometries/SphereGeometry.h"

struct brdfParams {
    glm::vec3 albedo = glm::vec3(0.7f);
    float metallic = 0.0f;
    float roughness = 0.7f;
    float ao = 1.0f;
} brdfPara;
struct subsurfaceParams {
    glm::vec3 color = glm::vec3(0.3, 0.8, 0.4);
    float power = 3.0f;
    float thickness = 0.8f;
    float scale = 3.0f;
    float distortion = 0.5f;
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
    brdfMat->shader()->setVec3("brdf.albedo", glm::vec3(0.5f, 0.0f, 0.0f));
    brdfMat->shader()->setFloat("brdf.ao", 1.0f);

    auto createSphere =
        [&](std::shared_ptr<Material> brdf) -> std::unique_ptr<Object> {
        auto sphere = std::make_unique<Object>(brdf);
        auto sphereGeo = SphereGeometry::create(0.48);
        Primitive spherePrimitive = {sphereGeo,
                                     brdf->defaultInstance()->duplicate()};
        sphere->addComponent(std::move(
            std::make_unique<MeshComponent>(PrimitiveList{spherePrimitive})));
        return std::move(sphere);
    };

    // for PBR Test
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            auto sphere = createSphere(brdfMat);
            // sphere->setEnabled(false);
            sphere->transform()->setPosition({i - 3.5, j - 3.5, -5});
            m_spheres.emplace_back(std::move(sphere));
        }
    }

    std::string bsdf_fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/bsdf.fs");

    auto bsdfMat = Material::create(vs_path.c_str(), bsdf_fs_path.c_str());

    // for SSS Test
    std::string standfordDragonPath =
        std::string(ZEUS_ROOT_DIR).append("/model/Stanford/dragon.obj");

    auto dragon =
        std::make_unique<Object>(standfordDragonPath.c_str(), bsdfMat);
    dragon->transform()->setRotation({0, 1, 0}, -90.f);
    m_models.emplace_back(std::move(dragon));
}

static void gui() {
    ImGui::Begin("Material");
    if (ImGui::CollapsingHeader("PBR Controls")) {
        ImGui::Indent();
        ImGui::ColorEdit3("albedo", &brdfPara.albedo[0]);
        ImGui::SliderFloat("metallic", &brdfPara.metallic, 0.0f, 1.0f);
        ImGui::SliderFloat("roughness", &brdfPara.roughness, 0.05f, 1.0f);
        ImGui::SliderFloat("ao", &brdfPara.ao, 0.0f, 1.0f);
        ImGui::Unindent();
    }

    if (ImGui::CollapsingHeader("Subsurface Controls")) {
        ImGui::Indent();
        ImGui::SliderFloat("Thickness", &subsurfacePara.thickness, 0.0f, 1.0f);
        ImGui::SliderFloat("Power", &subsurfacePara.power, 0.5f, 10.0f);
        ImGui::SliderFloat("Scale", &subsurfacePara.scale, 0.0f, 5.0f);
        ImGui::SliderFloat("Distortion", &subsurfacePara.distortion, 0.0f,
                           1.0f);
        ImGui::ColorEdit3("Color", &subsurfacePara.color[0]);
        ImGui::Unindent();
    }
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

    auto &pbrShader = m_spheres[0]
                          ->getComponent<MeshComponent>()
                          ->primitives()[0]
                          .matInstance;

    pbrShader->use();
    pbrShader->setVec3("camPos",
                       Zeus::Engine::getInstance().camera().position());
    for (int i = 0; i < m_lights.size(); i++) {
        pbrShader->setVec3("lights[" + std::to_string(i) + "].pos",
                           m_lights[i]->transform()->position());
        pbrShader->setVec3("lights[" + std::to_string(i) + "].color",
                           lightColor);
    }

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            auto &sphere = m_spheres[i * 7 + j];
            // clamp to 0.05-1.0; 0粗糙度绝对光滑，看起来不太自然
            pbrShader->setFloat("brdf.roughness",
                                glm::clamp(float(i) / 7.f, 0.05f, 1.0f));
            pbrShader->setFloat("brdf.metallic", float(j) / 7.f);
            sphere->tick();
        }
    }

    auto &sssShader =
        m_models[0]->getComponent<MeshComponent>()->primitives()[0].matInstance;
    sssShader->use();
    sssShader->setVec3("camPos",
                       Zeus::Engine::getInstance().camera().position());
    for (int i = 0; i < m_lights.size(); i++) {
        sssShader->setVec3("lights[" + std::to_string(i) + "].pos",
                           m_lights[i]->transform()->position());
        sssShader->setVec3("lights[" + std::to_string(i) + "].color",
                           lightColor);
    }

    for (auto &sphere : m_models) {
        sssShader->setFloat("brdf.roughness", brdfPara.roughness);
        sssShader->setFloat("brdf.metallic", brdfPara.metallic);
        sssShader->setVec3("brdf.albedo", brdfPara.albedo);
        sssShader->setFloat("brdf.ao", brdfPara.ao);
        sssShader->setFloat("subsurface.thickness", subsurfacePara.thickness);
        sssShader->setFloat("subsurface.power", subsurfacePara.power);
        sssShader->setVec3("subsurface.color", subsurfacePara.color);
        sssShader->setFloat("subsurface.scale", subsurfacePara.scale);
        sssShader->setFloat("subsurface.distortion", subsurfacePara.distortion);

        sphere->tick();
    }
}