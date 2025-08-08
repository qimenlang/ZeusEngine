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

    std::string ts_fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/transparent.fs");
    auto default_ts_mat = Material::create(vs_path.c_str(), ts_fs_path.c_str());

    std::string sample_diffuse_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/sampleDiffuse.fs");
    auto sample_diffuse_mat =
        Material::create(vs_path.c_str(), sample_diffuse_path.c_str());

    std::string blend_discard_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/blendDiscard.fs");
    auto grass_mat =
        Material::create(vs_path.c_str(), blend_discard_path.c_str());

    m_cube1 = std::make_unique<Object>(sample_diffuse_mat);
    m_cube1->setName("m_cube1");
    m_cube1->transform()->setPosition({0, 0.5, -2.0});

    Texture cube_texture;
    std::string cube_tex_path =
        std::string(ZEUS_ROOT_DIR).append("/texture/metal.png");
    cube_texture.id = TextureFromFile(cube_tex_path);
    cube_texture.type = "diffuse";

    auto cubeGeo = CubeGeometry::getDefault();
    cubeGeo.textures.push_back(cube_texture);
    Primitive cubePrimitive{cubeGeo,
                            sample_diffuse_mat->defaultInstance()->duplicate()};
    m_cube1->addComponent(std::move(
        std::make_unique<MeshComponent>(PrimitiveList{cubePrimitive})));
    auto &cube1Mat =
        m_cube1->getComponent<MeshComponent>()->primitives()[0].matInstance;

    Texture grass_texture;
    std::string grass_tex_path =
        std::string(ZEUS_ROOT_DIR).append("/texture/grass.png");
    grass_texture.id = TextureFromFile(grass_tex_path);
    grass_texture.type = "diffuse";
    auto addGrass = [&](glm::vec3 pos) -> std::unique_ptr<Object> {
        auto grass = std::make_unique<Object>(grass_mat);
        grass->setName("grass");
        grass->transform()->setPosition(pos);

        auto grassGeo = QuadGeometry::getDefault();
        grassGeo.textures.push_back(grass_texture);
        Primitive grassPrimitive{grassGeo,
                                 grass_mat->defaultInstance()->duplicate()};
        grass->addComponent(std::move(
            std::make_unique<MeshComponent>(PrimitiveList{grassPrimitive})));
        auto grassMat =
            grass->getComponent<MeshComponent>()->primitives()[0].matInstance;
        grassMat->setCullingMode(CullingMode::NONE);

        return grass;
    };

    Texture window_texture;
    std::string window_tex_path =
        std::string(ZEUS_ROOT_DIR)
            .append("/texture/blending_transparent_window.png");
    window_texture.id = TextureFromFile(window_tex_path);
    window_texture.type = "diffuse";

    auto addWindow = [&](glm::vec3 pos) -> std::unique_ptr<Object> {
        auto window = std::make_unique<Object>(sample_diffuse_mat);
        window->setName("window");
        window->transform()->setPosition(pos);

        auto windowGeo = QuadGeometry::getDefault();
        windowGeo.textures.push_back(window_texture);
        Primitive windowPrimitive{
            windowGeo, sample_diffuse_mat->defaultInstance()->duplicate()};
        window->addComponent(std::move(
            std::make_unique<MeshComponent>(PrimitiveList{windowPrimitive})));
        auto windowMat =
            window->getComponent<MeshComponent>()->primitives()[0].matInstance;
        windowMat->setCullingMode(CullingMode::NONE);
        return window;
    };

    auto cameraPos = Zeus::Engine::getInstance().camera().worldPosition();

    std::vector<glm::vec3> grassPositions = {
        {-0.3, 0.5, -0.7}, {0.3, 0.5, -1.0}, {0.1, 0.5, -1.3}};
    std::vector<glm::vec3> windowPositions = {
        {0.7, 0.5, -1.2}, {0.5, 0.5, -0.9}, {0.3, 0.5, -0.6}, {0.1, 0.5, -0.3}};
    for (auto pos : grassPositions) {
        m_ts_objs.emplace_back(std::move(addGrass(pos)));
    }
    for (auto pos : windowPositions) {
        m_ts_objs.emplace_back(std::move(addWindow(pos)));
    }

    std::string handPath = std::string(ZEUS_ROOT_DIR).append("/model/hand.obj");
    m_hand = std::make_unique<Object>(handPath.c_str(), default_ts_mat);
    m_hand->transform()->setPosition({0, 1.3, -0.5});
    m_hand->transform()->setScale(glm::vec3{0.01});
    auto &handMat =
        m_hand->getComponent<MeshComponent>()->primitives()[0].matInstance;
    handMat->setDepthFunc(SamplerCompareFunc::A);

    std::string cubePath = std::string(ZEUS_ROOT_DIR).append("/model/cube.obj");
    m_floor = std::make_unique<Object>(cubePath.c_str(), default_mat);
    m_floor->transform()->setScale(glm::vec3{10, 0.0, 10});

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void BlendScene::update() {
    auto cube1Color = glm::vec3{0.8, 0.2, 0.2};
    auto &cube1Mat =
        m_cube1->getComponent<MeshComponent>()->primitives()[0].matInstance;
    cube1Mat->use();
    m_cube1->tick();

    auto floorColor = glm::vec3{0.2, 0.2, 0.2};
    auto &floorMat =
        m_floor->getComponent<MeshComponent>()->primitives()[0].matInstance;
    floorMat->use();
    floorMat->setVec3("MatColor", floorColor);
    m_floor->tick();

    auto handColor = glm::vec3{0.8};
    auto &handMat =
        m_hand->getComponent<MeshComponent>()->primitives()[0].matInstance;
    handMat->use();
    handMat->setVec3("MatColor", handColor);
    handMat->setFloat("alpha", 0.5);
    m_hand->tick();

    auto sortByDistance = [](std::unique_ptr<Object> &obj1,
                             std::unique_ptr<Object> &obj2) -> bool {
        auto cameraPos = Zeus::Engine::getInstance().camera().worldPosition();
        float distance1 =
            glm::distance(cameraPos, obj1->transform()->position());
        float distance2 =
            glm::distance(cameraPos, obj2->transform()->position());
        return distance1 > distance2;
    };

    std::sort(m_ts_objs.begin(), m_ts_objs.end(), sortByDistance);

    // 最后按照到相机距离渲染所有透明物体
    for (auto itr = m_ts_objs.begin(); itr != m_ts_objs.end(); itr++) {
        auto &obj = *itr;
        auto mat =
            obj->getComponent<MeshComponent>()->primitives()[0].matInstance;
        mat->use();
        obj->tick();
    }
}
