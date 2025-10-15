#include "CubeScene.h"

#include <memory>

#include "Engine.h"
#include "function/framework/object/Object.h"
#include "function/render/Material.h"

CubeScene::CubeScene() : Scene() {
    // 初始化立方体场景
    std::cout << "CubeScene initialized." << std::endl;
}
CubeScene::~CubeScene() {
    // 清理立方体场景
    std::cout << "CubeScene destroyed." << std::endl;
}
void CubeScene::init() {
    // 初始化立方体场景中的模型和着色器
    std::cout << "CubeScene init called." << std::endl;
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
    auto cube_mat =
        Material::create(vs_path.c_str(), phong_sample_path.c_str());

    auto createCube = [&]() -> std::unique_ptr<Object> {
        auto cube = std::make_unique<Object>(cubePath.c_str(), cube_mat);
        cube->transform()->setScale(glm::vec3{0.48});
        return std::move(cube);
    };

    m_cube = createCube();
    m_cube->transform()->setPosition(glm::vec3{-2, 0, 0});
    m_cube1 = createCube();
    m_cube1->transform()->setPosition(glm::vec3{-1, 0, 0});
    m_cube2 = createCube();
    m_cube2->transform()->setPosition(glm::vec3{0, 0, 0});
    m_cube3 = createCube();
    m_cube3->transform()->setPosition(glm::vec3{1, 0, 0});
    m_cube4 = createCube();
    m_cube4->transform()->setPosition(glm::vec3{2, 0, 0});
}

void CubeScene::update() {
    auto lightColor = glm::vec3{1.0};
    auto &m_lightShader =
        m_lightCube->getComponent<MeshComponent>()->primitives()[0].matInstance;
    m_lightShader->use();
    m_lightShader->setVec3("lightColor", lightColor);
    m_lightCube->tick();

    auto &m_phongSampleShader =
        m_cube->getComponent<MeshComponent>()->primitives()[0].matInstance;

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

    m_phongSampleShader->setFloat("material.shininess", 2);
    m_cube->tick();
    m_phongSampleShader->setFloat("material.shininess", 8);
    m_cube1->tick();
    m_phongSampleShader->setFloat("material.shininess", 32);
    m_cube2->tick();
    m_phongSampleShader->setFloat("material.shininess", 128);
    m_cube3->tick();
    m_phongSampleShader->setFloat("material.shininess", 256);
    m_cube4->tick();
}