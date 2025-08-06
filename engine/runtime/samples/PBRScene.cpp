#include "PBRScene.h"

#include <memory>

#include "Engine.h"
#include "function/framework/component/MeshComponent.h"
#include "function/framework/object/Object.h"
#include "function/render/Material.h"
#include "resource/geometries/SphereGeometry.h"

PBRScene::PBRScene() : Scene() {
    // 初始化立方体场景
    std::cout << "PBRScene initialized." << std::endl;
}
PBRScene::~PBRScene() {
    // 清理立方体场景
    std::cout << "PBRScene destroyed." << std::endl;
}
void PBRScene::init() {
    // 初始化立方体场景中的模型和着色器
    std::cout << "PBRScene init called." << std::endl;
    // 可以在这里添加模型、着色器等初始化代码

    std::string vs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/default.vs");
    std::string ls_path = std::string(ZEUS_ROOT_DIR).append("/shader/light.fs");

    auto light_mat = Material::create(vs_path.c_str(), ls_path.c_str());

    std::string lightCubePath =
        std::string(ZEUS_ROOT_DIR).append("/model/cube.obj");
    m_lightCube = std::make_unique<Object>(lightCubePath.c_str(), light_mat);
    m_lightCube->onTick.add([](Object *thiz) {
        thiz->transform()->setPosition(glm::vec3{
            20 * sin(Zeus::Engine::getInstance().currentTime()), 0.0, 2.0});
        thiz->transform()->setRotation(glm::vec3(1.f, 0.0f, 0.0f), 0.f);
        thiz->transform()->setScale(glm::vec3(0.04f));
    });

    std::string phong_sample_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/phongSampleColor.fs");

    std::string cubePath =
        std::string(ZEUS_ROOT_DIR).append("/model/cubeWithNormal.obj");
    auto sphere_mat =
        Material::create(vs_path.c_str(), phong_sample_path.c_str());

    auto createSphere = [&]() -> std::unique_ptr<Object> {
        auto sphere = std::make_unique<Object>(sphere_mat);
        auto sphereGeo = SphereGeometry::create();
        Primitive spherePrimitive = {
            sphereGeo, sphere_mat->defaultInstance()->duplicate()};
        sphere->addComponent(std::move(
            std::make_unique<MeshComponent>(PrimitiveList{spherePrimitive})));

        sphere->transform()->setScale(glm::vec3{0.48});
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
    auto lightColor = glm::vec3{1.0};
    auto &m_lightShader =
        m_lightCube->getComponent<MeshComponent>()->primitives()[0].matInstance;
    m_lightShader->use();
    m_lightShader->setVec3("lightColor", lightColor);
    m_lightCube->tick();

    auto &m_phongSampleShader = m_spheres[0]
                                    ->getComponent<MeshComponent>()
                                    ->primitives()[0]
                                    .matInstance;

    m_phongSampleShader->use();
    m_phongSampleShader->setVec3("viewPos",
                                 Zeus::Engine::getInstance().camera().Position);
    m_phongSampleShader->setVec3("light.position",
                                 m_lightCube->transform()->position());
    m_phongSampleShader->setVec3("light.ambient", glm::vec3{0.2f});
    m_phongSampleShader->setVec3("light.diffuse", glm::vec3{0.5f});
    m_phongSampleShader->setVec3("light.specular", glm::vec3{1.0f});

    auto objectColor = glm::vec3{1.0f, 0.5f, 0.31f};
    m_phongSampleShader->setVec3("objectColor", objectColor);
    m_phongSampleShader->setVec3("material.ambient", objectColor);
    m_phongSampleShader->setVec3("material.diffuse", objectColor);
    m_phongSampleShader->setVec3("material.specular", glm::vec3{0.5f});

    for (int i = 0; i < m_spheres.size(); i++) {
        m_phongSampleShader->setFloat("material.shininess", i + 1);
        m_spheres[i]->tick();
    }
}