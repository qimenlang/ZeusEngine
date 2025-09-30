#include "ShadowScene.h"

#include <resource/geometries/CubeGeometry.h>
#include <resource/geometries/QuadGeometry.h>

#include <glm/gtx/string_cast.hpp>
#include <memory>

#include "Engine.h"
#include "function/framework/object/Object.h"
#include "function/render/Material.h"

const int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

ShadowScene::ShadowScene() : Scene() {
    std::cout << "ShadowScene initialized." << std::endl;
}
ShadowScene::~ShadowScene() {
    std::cout << "ShadowScene destroyed." << std::endl;
}
void ShadowScene::init() {
    std::cout << "ShadowScene init called." << std::endl;

    // 创建shadowMap FBO
    glGenFramebuffers(1, &fbo);
    // 创建深度纹理
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
                 SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // 绑定深度纹理到FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depthMap, 0);
    // 纹理不完整，不读写颜色缓冲
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
                  << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::string vs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/default.vs");
    std::string ls_path = std::string(ZEUS_ROOT_DIR).append("/shader/light.fs");
    std::string shadow_vs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/depth/shadowMap.vs");
    std::string shadow_fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/depth/shadowMap.fs");
    std::string sample_diffuse_fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/depth/phongWithShadow.fs");

    auto light_mat = Material::create(vs_path.c_str(), ls_path.c_str());

    std::string lightCubePath =
        std::string(ZEUS_ROOT_DIR).append("/model/cube.obj");
    m_lightCube = std::make_unique<Object>(lightCubePath.c_str(), light_mat);
    auto lightPos = glm::vec3{0.3f, 3.5f, 0.f};
    m_lightCube->transform()->setPosition(lightPos);
    m_lightCube->transform()->setScale(glm::vec3(0.1f));
    // m_lightCube->onTick.add([](Object *thiz) {
    //     thiz->transform()->setPosition(glm::vec3{
    //         2 * sin(Zeus::Engine::getInstance().currentTime()), 3.0, 0.0});
    // });

    float near_plane = 1.0f, far_plane = 7.5f;
    glm::mat4 lightProjection =
        glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f));
    lightSpaceMatrix = lightProjection * lightView;

    Texture cube_texture(
        std::string(ZEUS_ROOT_DIR).append("/texture/container.jpg"), "diffuse");
    Texture floor_texture(
        std::string(ZEUS_ROOT_DIR).append("/texture/metal.png"), "diffuse");
    Texture shadow_texture;
    shadow_texture.id = depthMap;
    shadow_texture.type = "shadowMap";

    auto shadow_mat =
        Material::create(shadow_vs_path.c_str(), shadow_fs_path.c_str());
    shadow_mat->use();
    shadow_mat->shader()->setMat4("lightSpaceMatrix", lightSpaceMatrix);

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
    cubeGeo.textures.push_back(shadow_texture);
    auto phong_mat =
        Material::create(vs_path.c_str(), sample_diffuse_fs_path.c_str());
    phong_mat->use();
    phong_mat->shader()->setVec3("light.ambient", glm::vec3{0.5f});
    phong_mat->shader()->setVec3("light.diffuse", glm::vec3{1.0f});
    phong_mat->shader()->setVec3("light.specular", glm::vec3{1.0f});
    phong_mat->shader()->setVec3("material.specular", glm::vec3{0.5f});
    phong_mat->shader()->setFloat("material.shininess", 32);

    vector<glm::vec3> positions{
        {-2, 0, -1}, {-1, 2, 0}, {0, 1, -0.5}, {1, 1.5, -1}, {2, 0.5, 0.5}};
    for (const auto &pos : positions) {
        m_shadowObjects.emplace_back(createObj(cubeGeo, shadow_mat, pos));
        m_objects.emplace_back(createObj(cubeGeo, phong_mat, pos));
    };

    auto floorGeo = CubeGeometry::getDefault();
    floorGeo.textures.push_back(floor_texture);
    auto shadow_floor = createObj(floorGeo, shadow_mat, glm::vec3{0, -0.26, 0});
    shadow_floor->transform()->setScale(glm::vec3{30, 0.01, 30});
    auto floor = createObj(floorGeo, phong_mat, glm::vec3{0, -0.26, 0});
    floor->transform()->setScale(glm::vec3{10, 0.01, 10});
    m_shadowObjects.emplace_back(std::move(shadow_floor));
    m_objects.emplace_back(std::move(floor));

    std::string quad_vs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/screen/fb_screen.vs");
    std::string quad_fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/depth/quadDebugDepth.fs");

    auto quad_mat =
        Material::create(quad_vs_path.c_str(), quad_fs_path.c_str());
    quad_mat->shader()->use();
    quad_mat->shader()->setFloat("near_plane", near_plane);
    quad_mat->shader()->setFloat("far_plane", far_plane);

    auto quadGeo = QuadGeometry::getDefault(QuadGeometryType::ScreenQuad);
    quadGeo.textures.push_back(shadow_texture);
    for (auto &vertex : quadGeo.vertices) {
        vertex.Position = vertex.Position * 0.2f + glm::vec3{0.8f};
    }
    m_quad = createObj(quadGeo, quad_mat, glm::vec3{0, 0, -1});
}

void ShadowScene::update() {
    // 没有SubPass 功能，暂时通过场景中所有物体都有两个对象的方式实现阴影
    // 每个物体的第一个对象带阴影材质，用于生成shadow map
    // 物体的第二个对象带phong材质并计算阴影,用于真正渲染场景
    // 物体的两个对象transform必须完全一致；

    // 1.渲染shadow map
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &object : m_shadowObjects) {
        object->tick();
    }

    // 2.解绑 shadow map
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Zeus::SCR_WIDTH, Zeus::SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 3.渲染场景
    auto &lightShader =
        m_lightCube->getComponent<MeshComponent>()->primitives()[0].matInstance;
    lightShader->use();
    lightShader->setVec3("lightColor", glm::vec3{1, 1, 1});
    m_lightCube->tick();

    for (auto &object : m_objects) {
        auto &phongShader =
            object->getComponent<MeshComponent>()->primitives()[0].matInstance;
        phongShader->use();
        phongShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        phongShader->setVec3("viewPos",
                             Zeus::Engine::getInstance().camera().Position);
        phongShader->setVec3("light.position",
                             m_lightCube->transform()->position());
        object->tick();
    }
    // 4.渲染debug shadow map
    auto quadShader =
        m_quad->getComponent<MeshComponent>()->primitives()[0].matInstance;
    m_quad->tick();
}