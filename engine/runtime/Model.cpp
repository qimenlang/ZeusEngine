#include "Model.h"

#include <memory>

#include "Engine.h"
#include "resource/asset_manager.h"

void Model::Draw() {
    if (m_shader.expired()) {
        std::cerr << "Model shader is expired!" << std::endl;
        return;
    }
    m_shader.lock()->use();
    m_shader.lock()->setMat4("model", m_transform->GetModelMatrix());

    auto cameraPos = Zeus::Engine::getInstance().camera().Position;

    // std::cout << "Model Camera Position: (" << cameraPos.x << ", "
    //           << cameraPos.y << ", " << cameraPos.z << ")"
    //           << " ,camera :" << &Zeus::Engine::getInstance().camera()
    //           << std::endl;

    m_shader.lock()->setMat4(
        "view", Zeus::Engine::getInstance().camera().GetViewMatrix());
    m_shader.lock()->setMat4("projection", Zeus::projection);

    m_mesh_component->tick(0);
    m_transform->tick(0);
}
