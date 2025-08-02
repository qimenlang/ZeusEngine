#include "ModelScene.h"

#include <glm/gtx/string_cast.hpp>
#include <memory>

#include "Engine.h"
#include "function/framework/object/Object.h"

ModelScene::ModelScene() : Scene() {
    // 初始化立方体场景
    std::cout << "ModelScene initialized." << std::endl;
}
ModelScene::~ModelScene() {
    // 清理立方体场景
    std::cout << "ModelScene destroyed." << std::endl;
}
void ModelScene::init() {
    // 初始化立方体场景中的模型和着色器
    std::cout << "ModelScene init called." << std::endl;
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
            2 * sin(Zeus::Engine::getInstance().currentTime()), 0.0, 2.0});
        thiz->transform()->setRotation(glm::vec3(1.f, 0.0f, 0.0f), 0.f);
        thiz->transform()->setScale(glm::vec3(0.04f));
    });

    std::string phong_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/phong.fs");
    std::string DragonPath =
        std::string(ZEUS_ROOT_DIR).append("/model/dragon/Dragon 2.5_fbx.fbx");
    auto dragon_mat = Material::create(vs_path.c_str(), phong_path.c_str());

    m_dragon = std::make_unique<Object>(DragonPath.c_str(), dragon_mat);
    std::cout << "Object Load :" << DragonPath << std::endl;
}

void ModelScene::update() {
    auto lightColor = glm::vec3{1.0};
    auto &lightShader =
        m_lightCube->getComponent<MeshComponent>()->primitives()[0].matInstance;
    lightShader->use();
    lightShader->setVec3("lightColor", lightColor);
    m_lightCube->tick();

    // be sure to activate the m_phongShader
    auto &m_phongShader =
        m_dragon->getComponent<MeshComponent>()->primitives()[0].matInstance;
    m_phongShader->use();
    m_phongShader->setVec3("viewPos",
                           Zeus::Engine::getInstance().camera().Position);
    m_phongShader->setVec3("light.position",
                           m_lightCube->transform()->position());
    m_phongShader->setVec3("light.ambient", glm::vec3{0.2f});
    m_phongShader->setVec3("light.diffuse", glm::vec3{1.0f});
    m_phongShader->setVec3("light.specular", glm::vec3{1.0f});
    m_phongShader->setVec3("material.specular", glm::vec3{0.5f});
    m_phongShader->setFloat("material.shininess", 32);

    m_dragon->transform()->setPosition(glm::vec3{0, 0.5, 0});
    m_dragon->transform()->setRotation(glm::vec3(1.f, 0.0f, 0.0f), 180.f);
    m_dragon->transform()->setScale(glm::vec3{0.01});
    m_dragon->tick();
}