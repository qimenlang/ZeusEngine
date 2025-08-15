#include "CubeFBOScene.h"

#include <resource/geometries/CubeGeometry.h>
#include <resource/geometries/QuadGeometry.h>

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
    Texture floor_texture(
        std::string(ZEUS_ROOT_DIR).append("/texture/metal.png"), "diffuse");

    auto createObj = [&](Geometry &geo, std::shared_ptr<Material> mat,
                         glm::vec3 position) -> std::unique_ptr<Object> {
        auto quad = std::make_unique<Object>(mat);
        quad->addComponent<MeshComponent>(
            PrimitiveList{{geo, mat->defaultInstance()->duplicate()}});
        quad->transform()->setScale(glm::vec3{0.5f});
        quad->transform()->setPosition(position);
        return std::move(quad);
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
    floor->transform()->setScale(glm::vec3{5, 0.01, 5});
    m_objects.emplace_back(std::move(floor));

    auto quadGeo = QuadGeometry::getDefault(QuadGeometryType::ScreenQuad);
    quadGeo.textures.push_back(cube_texture);
    m_quad = createObj(quadGeo, cube_mat, glm::vec3{0, 0, -1});

    // init fbo
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // color attachment texture
    // 创建一个纹理来存储颜色缓冲区
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Zeus::SCR_WIDTH, Zeus::SCR_HEIGHT, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           textureColorbuffer, 0);
    // 创建深度模板缓冲区
    unsigned int rbo;
    glGenFramebuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Zeus::SCR_WIDTH,
                          Zeus::SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, rbo);
    // 检查帧缓冲区是否完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
                  << std::endl;
    // 解绑帧缓冲区
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CubeFBOScene::update() {
    auto &m_sampleShader = m_objects[0]
                               ->getComponent<MeshComponent>()
                               ->primitives()[0]
                               .matInstance;

    m_sampleShader->use();
    for (auto &cube : m_objects) cube->tick();

    auto &m_quadShader =
        m_quad->getComponent<MeshComponent>()->primitives()[0].matInstance;
    m_quadShader->use();
    m_quad->tick();
}