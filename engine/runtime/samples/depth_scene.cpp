#include "depth_scene.h"

#include <glm/gtx/string_cast.hpp>
#include <memory>

#include "Engine.h"
#include "function/framework/object/object.h"

void DepthScene::init() {
    Zeus::Engine::getInstance().camera().setWorldPosition(glm::vec3{0, 1, 3});
    std::string vs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/default.vs");
    std::string fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/default.fs");
    auto default_mat = Material::create(vs_path.c_str(), fs_path.c_str());

    std::string cubePath = std::string(ZEUS_ROOT_DIR).append("/model/cube.obj");
    m_cube1 = std::make_unique<Object>(cubePath.c_str(), default_mat);
    m_cube1->setName("m_cube1");
    m_cube1->transform()->setPosition({-0.5, 0.5, 0});
    auto &cube1Mat =
        m_cube1->getComponent<MeshComponent>()->primitives()[0].matInstance;
    cube1Mat->setDepthTest(false);

    m_cube2 = std::make_unique<Object>(cubePath.c_str(), default_mat);
    m_cube2->setName("m_cube2");
    m_cube2->transform()->setPosition({0.0, 0.5, -0.5});
    auto &cube2Mat =
        m_cube2->getComponent<MeshComponent>()->primitives()[0].matInstance;

    m_cube3 = std::make_unique<Object>(cubePath.c_str(), default_mat);
    m_cube3->setName("m_cube3");
    m_cube3->transform()->setPosition({0.5, 0.5, -1});
    auto &cube3Mat =
        m_cube3->getComponent<MeshComponent>()->primitives()[0].matInstance;
    cube3Mat->setDepthWirte(false);

    m_floor = std::make_unique<Object>(cubePath.c_str(), default_mat);
    m_floor->transform()->setScale(glm::vec3{10, 0.0, 10});

    auto &floorMat =
        m_floor->getComponent<MeshComponent>()->primitives()[0].matInstance;
    // floorMat->setDepthTest(false);
    // floorMat->setDepthWirte(false);
}

void DepthScene::update() {
    auto cube1Color = glm::vec3{0.8, 0.2, 0.2};
    auto &cube1Mat =
        m_cube1->getComponent<MeshComponent>()->primitives()[0].matInstance;
    cube1Mat->use();
    cube1Mat->setVec3("MatColor", cube1Color);
    m_cube1->tick();

    auto cube2Color = glm::vec3{0.2, 0.8, 0.2};
    auto &cube2Mat =
        m_cube2->getComponent<MeshComponent>()->primitives()[0].matInstance;
    cube2Mat->use();
    cube2Mat->setVec3("MatColor", cube2Color);
    m_cube2->tick();

    auto cube3Color = glm::vec3{0.2, 0.2, 0.8};
    auto &cube3Mat =
        m_cube3->getComponent<MeshComponent>()->primitives()[0].matInstance;
    cube3Mat->use();
    cube3Mat->setVec3("MatColor", cube3Color);
    m_cube3->tick();

    auto floorColor = glm::vec3{0.2, 0.2, 0.2};
    auto &floorMat =
        m_floor->getComponent<MeshComponent>()->primitives()[0].matInstance;
    floorMat->use();
    floorMat->setVec3("MatColor", floorColor);
    m_floor->tick();
}
