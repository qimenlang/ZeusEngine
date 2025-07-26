#include "BlendScene.h"

#include <glm/gtx/string_cast.hpp>
#include <memory>

#include "Engine.h"
#include "function/framework/object/Object.h"
#include "resource/geometries/CubeGeometry.h"

void BlendScene::init() {
    Zeus::Engine::getInstance().camera().setWorldPosition(glm::vec3{0, 1, 3});
    std::string vs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/default.vs");
    std::string fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/default.fs");
    auto default_mat = Material::create(vs_path.c_str(), fs_path.c_str());

    std::string cubePath = std::string(ZEUS_ROOT_DIR).append("/model/cube.obj");
    m_cube1 = std::make_unique<Object>(default_mat);
    m_cube1->setName("m_cube1");
    m_cube1->transform()->setPosition({-0.5, 0.5, 0});

    Primitive cubePrimitive{CubeGeometry::getDefault(),
                            default_mat->defaultInstance()->duplicate()};
    auto meshComp =
        std::make_unique<MeshComponent>(PrimitiveList{cubePrimitive});
    m_cube1->addComponent(std::move(meshComp));

    m_floor = std::make_unique<Object>(cubePath.c_str(), default_mat);
    m_floor->transform()->setScale(glm::vec3{10, 0.0, 10});
}

void BlendScene::update() {
    auto cube1Color = glm::vec3{0.8, 0.2, 0.2};
    auto &cube1Mat =
        m_cube1->getComponent<MeshComponent>()->primitives()[0].matInstance;
    cube1Mat->use();
    cube1Mat->setVec3("MatColor", cube1Color);
    m_cube1->tick();

    auto floorColor = glm::vec3{0.2, 0.2, 0.2};
    auto &floorMat =
        m_floor->getComponent<MeshComponent>()->primitives()[0].matInstance;
    floorMat->use();
    floorMat->setVec3("MatColor", floorColor);
    m_floor->tick();
}
