#include "PBRTextureScene.h"

#include <memory>

#include "Engine.h"
#include "function/framework/component/MeshComponent.h"
#include "function/framework/object/Object.h"
#include "function/render/Material.h"
#include "resource/geometries/SphereGeometry.h"

PBRTextureScene::PBRTextureScene() : Scene() {
    std::cout << "PBRTextureScene initialized." << std::endl;
}

PBRTextureScene::~PBRTextureScene() {
    std::cout << "PBRTextureScene destroyed." << std::endl;
}

void PBRTextureScene::init() {
    std::cout << "PBRTextureScene init called." << std::endl;
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

    m_lights.emplace_back(addLight({0, 0, 2.0}));

    std::string pbr_fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/pbrWithTextures.fs");

    auto pbr_sphere_mat =
        Material::create(vs_path.c_str(), pbr_fs_path.c_str());
    pbr_sphere_mat->shader()->use();
    pbr_sphere_mat->shader()->setVec3("mat.albedo",
                                      glm::vec3(0.5f, 0.0f, 0.0f));
    pbr_sphere_mat->shader()->setFloat("mat.ao", 1.0f);

    // UGLY！！！TO BE REFACTORED
    auto albedoID =
        TextureFromFile(std::string(ZEUS_ROOT_DIR)
                            .append("/texture/pbr/rusted_iron/albedo.png")
                            .c_str());
    Texture tex_albedo = Texture{albedoID, "albedo"};
    auto aoID = TextureFromFile(std::string(ZEUS_ROOT_DIR)
                                    .append("/texture/pbr/rusted_iron/ao.png")
                                    .c_str());
    Texture tex_ao = Texture{aoID, "ao"};
    auto metallicID =
        TextureFromFile(std::string(ZEUS_ROOT_DIR)
                            .append("/texture/pbr/rusted_iron/metallic.png")
                            .c_str());
    Texture tex_metallic = Texture{metallicID, "metallic"};
    auto normalID =
        TextureFromFile(std::string(ZEUS_ROOT_DIR)
                            .append("/texture/pbr/rusted_iron/normal.png")
                            .c_str());
    Texture tex_normal = {normalID, "normal"};
    auto roughnessID =
        TextureFromFile(std::string(ZEUS_ROOT_DIR)
                            .append("/texture/pbr/rusted_iron/roughness.png")
                            .c_str());
    Texture tex_roughness = {roughnessID, "roughness"};

    auto createSphere = [&]() -> std::unique_ptr<Object> {
        auto sphere = std::make_unique<Object>(pbr_sphere_mat);
        auto sphereGeo = SphereGeometry::create(0.48);
        sphereGeo.textures = {tex_albedo, tex_ao, tex_metallic, tex_normal,
                              tex_roughness};
        Primitive spherePrimitive = {
            sphereGeo, pbr_sphere_mat->defaultInstance()->duplicate()};
        sphere->addComponent(std::move(
            std::make_unique<MeshComponent>(PrimitiveList{spherePrimitive})));
        return std::move(sphere);
    };

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            auto sphere = createSphere();
            sphere->transform()->setPosition({i - 3.5, j - 3.5, -5});
            m_spheres.emplace_back(std::move(sphere));
        }
    }
}

void PBRTextureScene::update() {
    auto lightColor = glm::vec3(150.0f);
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

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            auto &sphere = m_spheres[i * 7 + j];
            pbrShader->setFloat("mat.roughness", float(i) / 7.0);
            pbrShader->setFloat("mat.metallic", float(j) / 7.0);
            sphere->tick();
        }
    }
}