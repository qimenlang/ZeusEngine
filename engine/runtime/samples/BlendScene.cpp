#include "BlendScene.h"

#include <glm/gtx/string_cast.hpp>
#include <memory>

#include "Engine.h"
#include "function/framework/object/Object.h"
#include "resource/geometries/CubeGeometry.h"
#include "resource/geometries/QuadGeometry.h"

void BlendScene::init() {
    Zeus::Engine::getInstance().camera().setWorldPosition(glm::vec3{0, 1, 3});
    std::string vs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/default.vs");
    std::string fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/default.fs");
    auto default_mat = Material::create(vs_path.c_str(), fs_path.c_str());

    std::string sample_diffuse_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/sampleDiffuse.fs");
    auto cube_mat =
        Material::create(vs_path.c_str(), sample_diffuse_path.c_str());

    m_cube1 = std::make_unique<Object>(cube_mat);
    m_cube1->setName("m_cube1");
    m_cube1->transform()->setPosition({0, 0.5, 0});

    Texture cube_texture;
    std::string cube_tex_path =
        std::string(ZEUS_ROOT_DIR).append("/texture/metal.png");
    cube_texture.id = TextureFromFile(cube_tex_path);
    cube_texture.type = "texture_diffuse";

    auto cubeGeo = CubeGeometry::getDefault();
    cubeGeo.textures.push_back(cube_texture);
    Primitive cubePrimitive{cubeGeo, cube_mat->defaultInstance()->duplicate()};
    m_cube1->addComponent(std::move(
        std::make_unique<MeshComponent>(PrimitiveList{cubePrimitive})));

    Texture grass_texture;
    std::string grass_tex_path =
        std::string(ZEUS_ROOT_DIR).append("/texture/grass.png");
    grass_texture.id = TextureFromFile(grass_tex_path);
    grass_texture.type = "texture_diffuse";

    // TODO:共用了同一个材质，待重构
    m_grass = std::make_unique<Object>(cube_mat);
    m_grass->setName("m_grass");
    m_grass->transform()->setPosition({0.1, 0.5, 0.8});

    auto grassGeo = QuadGeometry::getDefault();
    grassGeo.textures.push_back(grass_texture);
    Primitive grassPrimitive{grassGeo,
                             cube_mat->defaultInstance()->duplicate()};
    m_grass->addComponent(std::move(
        std::make_unique<MeshComponent>(PrimitiveList{grassPrimitive})));
    auto grassMat =
        m_grass->getComponent<MeshComponent>()->primitives()[0].matInstance;
    grassMat->setCullingMode(CullingMode::NONE);

    std::string cubePath = std::string(ZEUS_ROOT_DIR).append("/model/cube.obj");
    m_floor = std::make_unique<Object>(cubePath.c_str(), default_mat);
    m_floor->transform()->setScale(glm::vec3{10, 0.0, 10});
}

void BlendScene::update() {
    auto cube1Color = glm::vec3{0.8, 0.2, 0.2};
    auto &cube1Mat =
        m_cube1->getComponent<MeshComponent>()->primitives()[0].matInstance;
    cube1Mat->use();
    m_cube1->tick();

    auto grassMat =
        m_grass->getComponent<MeshComponent>()->primitives()[0].matInstance;
    grassMat->use();
    m_grass->tick();

    auto floorColor = glm::vec3{0.2, 0.2, 0.2};
    auto &floorMat =
        m_floor->getComponent<MeshComponent>()->primitives()[0].matInstance;
    floorMat->use();
    floorMat->setVec3("MatColor", floorColor);
    m_floor->tick();
}
