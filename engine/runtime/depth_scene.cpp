// #include "depth_scene.h"

// #include <glm/gtx/string_cast.hpp>
// #include <memory>

// #include "Engine.h"
// #include "function/framework/object/object.h"
// #include "function/render/Shader.h"

// void DepthScene::init() {
//     std::string vs_path =
//         std::string(ZEUS_ROOT_DIR).append("/shader/default.vs");
//     std::string ls_path =
//     std::string(ZEUS_ROOT_DIR).append("/shader/light.fs");

//     m_cubeShader = std::make_shared<Shader>(vs_path.c_str(),
//     ls_path.c_str());

//     std::string lightCubePath =
//         std::string(ZEUS_ROOT_DIR).append("/model/cube.obj");
//     m_cube = std::make_unique<Object>(lightCubePath.c_str());
//     m_cube->setShader(m_cubeShader);
// }

// void DepthScene::update() {
//     auto lightColor = glm::vec3{1.0};
//     m_cubeShader->use();
//     m_cubeShader->setVec3("lightColor", lightColor);

//     m_cube->transform()->setPosition(glm::vec3{
//         2 * sin(Zeus::Engine::getInstance().currentTime()), 0.0, 2.0});
//     m_cube->transform()->setRotation(glm::vec3(1.f, 0.0f, 0.0f), 0.f);
//     m_cube->transform()->setScale(glm::vec3(0.04f));
//     m_cube->tick();
// }
