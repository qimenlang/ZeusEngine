#include "CubeFBOScene.h"

#include <resource/geometries/CubeGeometry.h>

#include <memory>

#include "Engine.h"
#include "function/framework/object/Object.h"
#include "function/render/Material.h"

CubeFBOScene::CubeFBOScene() : Scene() {
    // 初始化立方体场景
    std::cout << "CubeFBOScene initialized." << std::endl;
}
CubeFBOScene::~CubeFBOScene() {
    // 清理立方体场景
    std::cout << "CubeFBOScene destroyed." << std::endl;
}
void CubeFBOScene::init() {
    // 初始化立方体场景中的模型和着色器
    std::cout << "CubeFBOScene init called." << std::endl;
    // 可以在这里添加模型、着色器等初始化代码

    std::string vs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/default.vs");

    std::string sample_diffuse_fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/sampleDiffuse.fs");
    auto cube_mat =
        Material::create(vs_path.c_str(), sample_diffuse_fs_path.c_str());
    Texture cube_texture(
        std::string(ZEUS_ROOT_DIR).append("/texture/container.jpg"), "diffuse");

    auto createCube = [&](glm::vec3 position) -> std::unique_ptr<Object> {
        auto cube = std::make_unique<Object>(cube_mat);
        auto cubeGeo = CubeGeometry::getDefault();
        cubeGeo.textures.push_back(cube_texture);

        cube->addComponent<MeshComponent>(
            PrimitiveList{{cubeGeo, cube_mat->defaultInstance()->duplicate()}});

        cube->transform()->setScale(glm::vec3{0.48});
        cube->transform()->setPosition(position);
        return std::move(cube);
    };

    vector<glm::vec3> positions{
        {-2, 0, 0}, {-1, 0, 0}, {0, 0, 0}, {1, 0, 0}, {2, 0, 0}};
    for (const auto &pos : positions) {
        m_cubes.emplace_back(createCube(pos));
    };
}

void CubeFBOScene::update() {
    auto &m_sampleShader =
        m_cubes[0]->getComponent<MeshComponent>()->primitives()[0].matInstance;

    m_sampleShader->use();
    for (auto &cube : m_cubes) cube->tick();
}