#include "stencil_scene.h"

#include <glm/gtx/string_cast.hpp>
#include <memory>

#include "Engine.h"
#include "function/framework/object/object.h"

void StencilScene::init() {
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

    m_cube2 = std::make_unique<Object>(cubePath.c_str(), default_mat);
    m_cube2->setName("m_cube2");
    m_cube2->transform()->setPosition({-0.5, 0.5, 0});

    m_floor = std::make_unique<Object>(cubePath.c_str(), default_mat);
    m_floor->transform()->setScale(glm::vec3{10, 0.0, 10});
    auto &floorMat =
        m_floor->getComponent<MeshComponent>()->primitives()[0].matInstance;

    glEnable(GL_STENCIL_TEST);
    // glStencilFunc(GL_ALWAYS, 1, 0xFF);
    // glStencilMask(0xFF);  // 关闭模板写入
    // 模板测试失败、深度测试失败、模板深度测试成功时，对模板缓冲的操作
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void StencilScene::update() {
    // 关闭模板写入
    glStencilMask(0x00);
    // 正常绘制floor
    auto floorColor = glm::vec3{0.2, 0.2, 0.2};
    auto &floorMat =
        m_floor->getComponent<MeshComponent>()->primitives()[0].matInstance;
    floorMat->use();
    floorMat->setVec3("MatColor", floorColor);
    m_floor->tick();

    // 正常绘制两个cube,同时写入模板缓冲
    glStencilMask(0xFF);  // 开启模板写入
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    auto cube1Color = glm::vec3{0.8, 0.2, 0.2};
    auto &cube1Mat =
        m_cube1->getComponent<MeshComponent>()->primitives()[0].matInstance;
    cube1Mat->use();
    cube1Mat->setVec3("MatColor", cube1Color);
    m_cube1->tick();

    // 绘制扩大的cube
    glStencilMask(0x00);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    auto edgeColor = glm::vec3{0.9, 0.9, 0.9};
    auto &cube2Mat =
        m_cube2->getComponent<MeshComponent>()->primitives()[0].matInstance;
    cube2Mat->use();
    cube2Mat->setVec3("MatColor", edgeColor);
    cube2Mat->setDepthTest(false);
    m_cube2->transform()->setScale(glm::vec3{1.1});
    m_cube2->tick();

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
}
