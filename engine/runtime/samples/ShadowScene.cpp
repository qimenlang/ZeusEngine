#include "ShadowScene.h"

#include <resource/geometries/CubeGeometry.h>
#include <resource/geometries/QuadGeometry.h>

#include <memory>

#include "Engine.h"
#include "function/framework/object/Object.h"
#include "function/render/Material.h"

ShadowScene::ShadowScene() : Scene() {
    std::cout << "ShadowScene initialized." << std::endl;
}
ShadowScene::~ShadowScene() {
    std::cout << "ShadowScene destroyed." << std::endl;
}
void ShadowScene::init() {
    std::cout << "ShadowScene init called." << std::endl;

    std::string vs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/default.vs");
    std::string ls_path = std::string(ZEUS_ROOT_DIR).append("/shader/light.fs");

    auto light_mat = Material::create(vs_path.c_str(), ls_path.c_str());

    std::string lightCubePath =
        std::string(ZEUS_ROOT_DIR).append("/model/cube.obj");
    m_lightCube = std::make_unique<Object>(lightCubePath.c_str(), light_mat);
    m_lightCube->transform()->setPosition(glm::vec3{0, 3.0, 0.0});
    m_lightCube->transform()->setScale(glm::vec3(0.1f));
    // m_lightCube->onTick.add([](Object *thiz) {
    //     thiz->transform()->setPosition(glm::vec3{
    //         2 * sin(Zeus::Engine::getInstance().currentTime()), 3.0, 0.0});
    // });

    std::string sample_diffuse_fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/sampleDiffuse.fs");
    auto cube_mat =
        Material::create(vs_path.c_str(), sample_diffuse_fs_path.c_str());
    Texture cube_texture(
        std::string(ZEUS_ROOT_DIR).append("/texture/container.jpg"), "diffuse");
    Texture floor_texture(
        std::string(ZEUS_ROOT_DIR).append("/texture/metal.png"), "diffuse");

    auto createObj = [&](Geometry &geo, std::shared_ptr<Material> mat,
                         glm::vec3 position) -> std::unique_ptr<Object> {
        auto obj = std::make_unique<Object>(mat);
        obj->addComponent<MeshComponent>(
            PrimitiveList{{geo, mat->defaultInstance()->duplicate()}});
        obj->transform()->setScale(glm::vec3{0.5f});
        obj->transform()->setPosition(position);
        return std::move(obj);
    };

    auto cubeGeo = CubeGeometry::getDefault();
    cubeGeo.textures.push_back(cube_texture);

    vector<glm::vec3> positions{
        {-2, 0, -1}, {-1, 2, 0}, {0, 1, -0.5}, {1, 1.5, -1}, {2, 0.5, 0.5}};
    for (const auto &pos : positions) {
        m_objects.emplace_back(createObj(cubeGeo, cube_mat, pos));
    };

    auto floorGeo = CubeGeometry::getDefault();
    floorGeo.textures.push_back(floor_texture);
    auto floor = createObj(floorGeo, cube_mat, glm::vec3{0, -0.51, 0});
    floor->transform()->setScale(glm::vec3{10, 0.01, 10});
    m_objects.emplace_back(std::move(floor));
}

void ShadowScene::update() {
    auto &lightShader =
        m_lightCube->getComponent<MeshComponent>()->primitives()[0].matInstance;
    lightShader->use();
    lightShader->setVec3("lightColor", glm::vec3{1, 1, 1});
    m_lightCube->tick();

    auto &objectShader = m_objects[0]
                             ->getComponent<MeshComponent>()
                             ->primitives()[0]
                             .matInstance;
    objectShader->use();
    for (auto &object : m_objects) object->tick();
}